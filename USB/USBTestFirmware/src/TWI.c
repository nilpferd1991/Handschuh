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
