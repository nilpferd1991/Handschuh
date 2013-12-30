/* Name: usb.c
 * Project: Handschuh Controller - die Firmware für den Handschuh
 * Author: Nils Braun
 */

#include "usb.h"
#include <util/delay.h>

// Deklaration der externen Daten
uint8_t sensorData[BUFFER_SIZE];
// Puffer mit der Nachricht nach draußen. Sie enthält maximal BUFFER_SIZE Zeichen.
unsigned char dataBuffer[BUFFER_SIZE];
uint8_t counter;


// I2C-Adresse des Beschleunigungssensors
// für Schreibe- und Lesevorgänge
#define SLA_W 0x70
#define SLA_R 0x71

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
//		sensorData[0] = (TWSR & 0xF8);
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
		sensorData[1] = (TWSR & 0xF8);
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
		sensorData[2] = (TWSR & 0xF8);
		return 0;
	}

	return 1;
}

inline int TW_SendStop() {
	// STOP senden. Wie immer TWEN und TWINT setzen. TWSTO steht für STOP-Signal.
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	return 1;
}


// Trennt die Verbindung zum Host für ca. 25 ms und meldet sich danach neu an
void usbForceDisconnect()
{
	// Re-enumeration forsieren während die Interrupts noch ausgeschalten sind
	usbDeviceDisconnect();

	// Ca. 250 ms warten
	_delay_ms(300);

	// Wieder anmelden und Interrupts anschalten
	usbDeviceConnect();

	sei();

	counter = 0;
}

// Diese Funktion wird immer dann aufgerufen, wenn eine neue Nachricht (vom Host) eintrifft
// setupData enthält die Kenngrößen der Nachricht.
usbMsgLen_t usbFunctionSetup(uchar setupData[8])
{
	// Setup-Nachricht vom Host ins richtige Format umwandeln
	usbRequest_t *request = (void*)setupData;

	// request->wLength enthält die geforderte Länge
	// request->wValue enthält den übergebenen Wert

	// Der RequestCode des Requests entscheidet über die Art der Programmausführung

	// ECHO
	if(request->bRequest == CUSTOM_RQ_ECHO)
	{
		// Sendet den Request zurück wie er gekommen ist
		// Dient zum Test der Funktionalität
		dataBuffer[0] = request->wLength.bytes[0];
		dataBuffer[1] = request->wLength.bytes[1];
		dataBuffer[2] = request->wValue.bytes[0];
		dataBuffer[3] = request->wValue.bytes[1];
		usbMsgPtr = dataBuffer;
		// Datenformat: [Länge]*2 [Wert]*2
		return CUSTOM_RQ_ECHO_LEN;
	}
	// DATA
	// Gebe die Sensordaten an den Host zurück
	else if(request->bRequest == CUSTOM_RQ_DATA)
	{

		usbMsgPtr = sensorData;


		// TWI START
		// Write: Read-Offset ab Adresse 02h (Daten)
//		if (!TW_SendStart() /*|| !TW_SendAdress(SLA_W) || !TW_SendData(0x02)*/ ) {
//			// ERROR
//			dataBuffer[0] = 1;
//			usbMsgPtr = dataBuffer;
//			TW_SendStop();
//			return 1;
//		}

//		// Read: Daten ab jetzt
//		if (!TW_SendStart() || !TW_SendAdress(SLA_R)) {
//			// ERROR
//			return 1;
//		}

//		// 5 Datensätze lesen, nach dem 6. abbrechen
//		for(int i = 0; i < 1; i++) {
//			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
//			// Auf Daten warten (TWINT muss wieder gesetzt sein)
//			while (!(TWCR & (1<<TWINT)));
//			sensorData[counter++] = TWDR;
//		}
//
//		// NACK
//		TWCR = (1 << TWINT) | (1 << TWEN);
//		// Auf Daten warten (TWINT muss wieder gesetzt sein)
//		while (!(TWCR & (1<<TWINT)));
//		sensorData[counter++] = TWDR;


	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

		uint64_t c = 0;
//		// Auf Übermittlung warten (TWINT muss wieder gesetzt sein)
		while  (!(TWCR & (1 << TWINT))) {
			wdt_reset();
		}

//
//		// In TWSR steht der Status der Übermittlung: Die ersten 5 Bits (0xF8) gebenden Status an.
//		if ((TWSR & 0xF8) != TW_START && (TWSR & 0xF8) != TW_REP_START) {
//			// Fehler
//	//		sensorData[0] = (TWSR & 0xF8);
//			return 0;
//		}

		TW_SendStop();

		counter = 0;

		// Datenformat: [Sensordaten]*BUFFER_SIZE
		return CUSTOM_RQ_DATA_LEN;
	}
	// Im allgemeinen Fall gibt es diese Nachricht gar nicht. Wir geben deshalb nichts zurück.
	
	return 0;
}
