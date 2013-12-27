/* Name: main.c
 * Project: Handschuh Controller - die Firmware für den Handschuh
 * Author: Nils Braun
 */

/*
 * Dies ist die Firmware für den Mikrocontroller. Sie steuert die
 * Kommunikation über USB und die AD-Wandlung. Nach der Initialisierung der Schnittstellen
 * wird in einem Zeitschritt der momentane Status der Sensoren abgefragt und dann
 * in den Puffer geschrieben. Werden die Daten vom USB-Host abgefragt, wird dieser Puffer gesendet.
 */

// I2C-Adresse des Beschleunigungssensors
// für Schreibe- und Lesevorgänge
#define SLA_W 0x70
#define SLA_R 0x71

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#include <string.h>

#include "usb.h"
#include "usbdrv/usbdrv.h"


int main(void)
{
	// Watchdog auf 1 Sekunde setzen, um bei einem Fehler sofort neuzustarten
	wdt_enable(WDTO_1S);

	// Timer 0 konfigurieren
	TCCR0 = (1<<CS01); // Prescaler 8

	// Overflow Interrupt erlauben
	TIMSK |= (1<<TOIE0);

	// USB initialisieren, Ports setzen
	usbInit();

	// Reenumeration forsieren
	usbForceDisconnect();


	// I2C initialisieren
	// PC0, PC1: Eingänge mit Pullup
	DDRC = 0;
	PORTC = (1 << 0) | (1 << 1);

	// USB: Nachrichtenschleife ausführen, watchdog zurücksetzen, USB-Nachrichten abrufen
	while(1)
	{
		wdt_reset();

		// Auf Nachricht warten
		usbPoll();
	}

	return 0;
}



// I2C-Funktionen
inline int TW_SendStart() {
	// START senden (TWEN setzt das I2C-Interface auf enabled. TWINT muss immer gesetzt werden,
	// um den TWINT-Flag zu clearen. TWSTA auf 1 bedeutet START.
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

	// Auf Übermittlung warten (TWINT muss wieder gesetzt sein)
	while  (!(TWCR & (1 << TWINT)));

	// In TWSR steht der Status der Übermittlung: Die ersten 5 Bits (0xF8) gebenden Status an.
	if ((TWSR & 0xF8) != TW_START && (TWSR & 0xF8) != TW_REP_START) {
		// Fehler
		//dataBuffer[0] = (TWSR & 0xF8);
		return 0;
	}

	return 1;
}

inline int TW_SendAdress(uint8_t adress) {
	// Adresse und Write-Bit senden. Nachricht steht in TWDR,
	// in TWCR muss wie immer TWEN und TWINT gesetzt werden
	TWDR = adress;
	TWCR = (1 << TWINT) | (1 << TWEN);

	// Auf Übermittlung warten (TWINT muss wieder gesetzt sein)
	while (!(TWCR & (1<<TWINT)));

	// In TWSR steht der Status der Übermittlung: Die ersten 5 Bits (0xF8) gebenden Status an.
	if ((TWSR & 0xF8) != TW_MT_SLA_ACK && (TWSR & 0xF8) != TW_MR_SLA_ACK) {
		// Fehler
		//dataBuffer[0] = (TWSR & 0xF8);
		return 0;
	}

	return 1;
}

inline int TW_SendData(uint8_t data) {
	// Daten senden. Nachricht steht in TWDR,
	// in TWCR muss wie immer TWEN und TWINT gesetzt werden
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// Auf Übermittlung warten (TWINT muss wieder gesetzt sein)
	while (!(TWCR & (1<<TWINT)));

	// In TWSR steht der Status der Übermittlung: Die ersten 5 Bits (0xF8) gebenden Status an.
	if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
		// Fehler
		//dataBuffer[0] = (TWSR & 0xF8);
		return 0;
	}

	return 1;
}

inline int TW_SendStop() {
	// STOP senden. Wie immer TWEN und TWINT setzen. TWSTO steht für STOP-Signal.
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	return 1;
}

ISR (TIMER0_OVF_vect)
{
	/* Interrupt Aktion alle
	(1000000/8)/256 Hz = 488,28125 Hz
	bzw.
	1/488,28125 s = 2,048 ms
	*/

	// TWI START, counter muss 0 sein, da sonst das letzte Mal zu wenig Daten abgerufen wurden
	if(counter == 0) {

		// Write: Read-Offset ab Adresse 02h (Daten)
		if (!TW_SendStart() || !TW_SendAdress(SLA_W) || !TW_SendData(0x02) ) {
			// ERROR
			return;
		}

		// Read: Daten ab jetzt
		if (!TW_SendStart() || !TW_SendAdress(SLA_R)) {
			// ERROR
			return;
		}

		// 5 Datensätze lesen, nach dem 6. abbrechen
		for(int i = 0; i < 5; i++) {
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
			// Auf Daten warten (TWINT muss wieder gesetzt sein)
			while (!(TWCR & (1<<TWINT)));
			sensorData[counter++] = TWDR;
		}

		// NACK
		TWCR = (1 << TWINT) | (1 << TWEN);
		// Auf Daten warten (TWINT muss wieder gesetzt sein)
		while (!(TWCR & (1<<TWINT)));
		sensorData[counter++] = TWDR;

		TW_SendStop();

		counter = 0;
	}
}
