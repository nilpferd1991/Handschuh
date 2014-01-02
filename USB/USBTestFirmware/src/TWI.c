#include "TWI.h"
#include <avr/interrupt.h>

#define DEBUG 0

uint8_t intCounter = 0;

void twiPoll() {

	// Nur etwas tun, falls auch etwas erhalten wurde.
	if(TWCR & (1 << TWINT)) {
		switch (TWSR & 0xF8) {
		// Start-Befehl angenommen. Nun senden wir die Write-Adresse SLA_W oder
		// Das 2. Start wurde angenommen. Nun können wir die Read-Adresse SLA_R senden
		case TW_REP_START:
		case TW_START:
			if(intCounter == 0) {
				if(DEBUG) sensorData[counter++] = 1;
				TWDR = SLA_W;
				TWCR = (1 << TWINT) | (1 << TWEN);
			}
			else if(intCounter == 1) {
				TWDR = SLA_R;
				if(DEBUG) sensorData[counter++] = 4;
				TWCR = (1 << TWINT) | (1 << TWEN);
			}
			break;

		// Write-Adresse angenommen. Nun senden wir wieder den Offset 0x02
		case TW_MT_SLA_ACK:
			if(DEBUG) sensorData[counter++] = 2;
			TWDR = 0x02;
			TWCR = (1 << TWINT) | (1 << TWEN);
			break;

		// Die Write-Adresse wurde nicht angenommen. Wir versuchen das Senden noch einmal
		// nur diesmal etwas schneller
		case TW_MT_SLA_NACK:
			TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWSTO);
			while(!(TWCR & (1 << TWINT))) ;
			TWDR = SLA_W;
			if(DEBUG) sensorData[counter++] = 0xf1;
			TWCR = (1 << TWINT) | (1 << TWEN);
			break;

		// Das Offset wurde angenommen. Nun können wir die Daten lesen. Also senden wir Start:
		case TW_MT_DATA_ACK:
			intCounter = 1;
			if(DEBUG) sensorData[counter++] = 3;
			TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
			break;

		// Die Read-Adresse wurde angenommen. Nun können wir Daten abholen:
		case TW_MR_SLA_ACK:
			if(DEBUG) sensorData[counter++] = 5;
			intCounter = 0;
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
			break;

		// Die Read-Adresse wurde nicht angenommen. Wir senden sie noch einmal,
		// nur diesmal schneller.
		case TW_MR_SLA_NACK:
			TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
			while(!(TWCR & (1 << TWINT))) ;
			if(DEBUG) sensorData[counter++] = 0xf5;
			TWDR = SLA_R;
			TWCR = (1 << TWINT) | (1 << TWEN);
			break;

		// Neue Daten sind angekommen. Wir rufen sie ab und fordern evtl neue an.
		case TW_MR_DATA_ACK:
			if(intCounter < 7) {
				sensorData[counter++] = TWDR;
				intCounter++;
				TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
			}
			else {
				if(DEBUG) sensorData[counter++] = 6;
				TWCR = (1 << TWINT) | (1 << TWEN);
			}
			break;

		// Das letzte Datenpaket ist angekommen, wir
		// fangen neu an!
		case TW_MR_DATA_NACK:
			if(DEBUG) sensorData[counter++] = 7;
			if(DEBUG) sensorData[0] = sensorData[0] + 1;
			counter = 0;
			intCounter = 0;
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA) | (1 << TWSTO);
			break;


		// Scheinbar ist etwas falsch gelaufen. Wir brechen ab.
		default:

			counter = 0;
			intCounter = 0;

			if(DEBUG) sensorData[counter++] = 0xff;
			if(DEBUG) sensorData[counter++] = TWSR & 0xF8;
			// Start/Stop senden
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA) | (1 << TWSTO);
			break;
		}
	}
}
