#include "TWI.h"
//
//int getData() {
//	// TWI START
//	// Write: Read-Offset ab Adresse 02h (Daten)
//	if (!TW_SendStart() || !TW_SendAdress(SLA_W) || !TW_SendData(0x02) ) {
//		// ERROR
//		return 0;
//	}
//
//	// Read: Daten ab jetzt
//	if (!TW_SendStart() || !TW_SendAdress(SLA_R)) {
//		// ERROR
//		return 0;
//	}
//
//	// 5 Datensätze lesen, nach dem 6. abbrechen
//	for(int i = 0; i < 5; i++) {
//		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
//		// Auf Daten warten (TWINT muss wieder gesetzt sein)
//		while (!(TWCR & (1<<TWINT)));
//		sensorData[counter++] = TWDR;
//	}
//
//	// NACK
//	TWCR = (1 << TWINT) | (1 << TWEN);
//	// Auf Daten warten (TWINT muss wieder gesetzt sein)
//	while (!(TWCR & (1<<TWINT)));
//	sensorData[counter++] = TWDR;
//
//	TW_SendStop();
//
//	counter = 0;
//
//	return 1;
//}

//
//// I2C-Funktionen
//inline int TW_SendStart() {
//	// START senden (TWEN setzt das I2C-Interface auf enabled. TWINT muss immer gesetzt werden,
//	// um den TWINT-Flag zu clearen. TWSTA auf 1 bedeutet START.
//	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
//
//	// Auf Übermittlung warten (TWINT muss wieder gesetzt sein)
//	while  (!(TWCR & (1 << TWINT)));
//
//	// In TWSR steht der Status der Übermittlung: Die ersten 5 Bits (0xF8) gebenden Status an.
//	if ((TWSR & 0xF8) != TW_START) {
//		// Fehler
//		return 0;
//	}
//
//	return 1;
//}
//
//inline int TW_SendRepStart() {
//	// START senden (TWEN setzt das I2C-Interface auf enabled. TWINT muss immer gesetzt werden,
//	// um den TWINT-Flag zu clearen. TWSTA auf 1 bedeutet START.
//	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
//
//	// Auf Übermittlung warten (TWINT muss wieder gesetzt sein)
//	while  (!(TWCR & (1 << TWINT)));
//
//	// In TWSR steht der Status der Übermittlung: Die ersten 5 Bits (0xF8) gebenden Status an.
//	if ((TWSR & 0xF8) != TW_REP_START) {
//		// Fehler
//		return 0;
//	}
//
//	return 1;
//}
//
//inline int TW_SendAdressWrite(uint8_t adress) {
//	// Adresse und Write-Bit senden. Nachricht steht in TWDR,
//	// in TWCR muss wie immer TWEN und TWINT gesetzt werden
//	TWDR = adress;
//	TWCR = (1 << TWINT) | (1 << TWEN);
//
//	// Auf Übermittlung warten (TWINT muss wieder gesetzt sein)
//	while (!(TWCR & (1<<TWINT)));
//
//	// In TWSR steht der Status der Übermittlung: Die ersten 5 Bits (0xF8) gebenden Status an.
//	if ((TWSR & 0xF8) != TW_MT_SLA_ACK/* && (TWSR & 0xF8) != TW_MR_SLA_ACK*/) {
//		// Fehler
//		return 0;
//	}
//
//	return 1;
//}
//
//inline int TW_SendAdressRead(uint8_t adress) {
//	// Adresse und Read-Bit senden. Nachricht steht in TWDR,
//	// in TWCR muss wie immer TWEN und TWINT gesetzt werden
//	TWDR = adress;
//	TWCR = (1 << TWINT) | (1 << TWEN);
//
//	// Auf Übermittlung warten (TWINT muss wieder gesetzt sein)
//	while (!(TWCR & (1<<TWINT)));
//
//	// In TWSR steht der Status der Übermittlung: Die ersten 5 Bits (0xF8) gebenden Status an.
//	if ((TWSR & 0xF8) != TW_MR_SLA_ACK) {
//		// Fehler
//		return 0;
//	}
//
//	return 1;
//}
//
//inline int TW_SendData(uint8_t data) {
//	// Daten senden. Nachricht steht in TWDR,
//	// in TWCR muss wie immer TWEN und TWINT gesetzt werden
//	TWDR = data;
//	TWCR = (1<<TWINT) | (1<<TWEN);
//
//	// Auf Übermittlung warten (TWINT muss wieder gesetzt sein)
//	while (!(TWCR & (1<<TWINT)));
//
//	// In TWSR steht der Status der Übermittlung: Die ersten 5 Bits (0xF8) gebenden Status an.
//	if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
//		// Fehler
//		return 0;
//	}
//
//	return 1;
//}
//
//inline int TW_SendStop() {
//	// STOP senden. Wie immer TWEN und TWINT setzen. TWSTO steht für STOP-Signal.
//	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
//	return 1;
//}


uint8_t intCounter = 0;

void twiPoll() {
	// Nur etwas tun, falls auch etwas erhalten wurde.
	if(TWCR & (1 << TWINT)) {
		switch (TWSR & 0xF8) {
		// Start-Befehl angenommen. Nun senden wir die Write-Adresse SLA_W
		case TW_START:
			if(intCounter == 0) {
				TWDR = SLA_W;
				TWCR = (1 << TWINT) | (1 << TWEN);
				sensorData[++counter] = 1;
				intCounter++;
			}
			break;
		// Write-Adresse angenommen. Nun senden wir wieder den Offset 0x02
		case TW_MT_SLA_ACK:
			TWDR = 0x02;
			TWCR = (1 << TWINT) | (1 << TWEN);
			sensorData[++counter] = 2;
			break;
		// Das Offset wurde angenommen. Nun können wir die Daten lesen. Also senden wir Start:
		case TW_MT_DATA_ACK:
			TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
			sensorData[++counter] = 3;
			break;
		// Das 2. Start wurde angenommen. Nun können wir die Read-Adresse SLA_R senden
		case TW_REP_START:
			TWDR = SLA_R;
			TWCR = (1 << TWINT) | (1 << TWEN);
			sensorData[++counter] = 4;
			break;
		// Die Read-Adresse wurde angenommen. Nun können wir Daten abholen:
		case TW_MR_SLA_ACK:
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
			sensorData[++counter] = 5;
			break;
		// Neue Daten sind angekommen. Wir rufen sie ab und fordern evtl neue an.
		case TW_MR_DATA_ACK:
			if(intCounter < 5) {
				TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
				sensorData[++counter] = 5;
				intCounter++;
			}
			else {
				TWCR = (1 << TWINT) | (1 << TWEN);
				sensorData[++counter] = 6;
			}
			break;
		// Das letzte Datenpaket ist angekommen, wir stoppen!
		case TW_MR_DATA_NACK:
			sensorData[++counter] = 7;
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
			counter = 0;
			intCounter = -1;
			break;
		// Scheinbar ist etwas falsch gelaufen. Wir fangen noch einmal an:
		default:
			sensorData[++counter] = TWSR & 0xF8;
			if(intCounter != -1) {
	//			for(int i = 0; i < BUFFER_SIZE; i++) {
	//				sensorData[i] = 0;
	//			}

				counter = 0;
	//			// Start/Stop senden
				TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
				intCounter = 0;
			}
			break;
		}
	}
}
