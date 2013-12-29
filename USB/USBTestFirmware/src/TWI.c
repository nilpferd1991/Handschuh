#include "TWI.h"

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

int getData() {
	// TWI START
	// Write: Read-Offset ab Adresse 02h (Daten)
	if (!TW_SendStart() || !TW_SendAdress(SLA_W) || !TW_SendData(0x02) ) {
		// ERROR
		return 0;
	}

	// Read: Daten ab jetzt
	if (!TW_SendStart() || !TW_SendAdress(SLA_R)) {
		// ERROR
		return 0;
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

	return 1;
}
