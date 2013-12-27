/* Name: usb.c
 * Project: Handschuh Controller - die Firmware für den Handschuh
 * Author: Nils Braun
 */


#include "usb.h"

// Deklaration der externen Daten
uint8_t sensorData[BUFFER_SIZE];
// Puffer mit der Nachricht nach draußen. Sie enthält maximal BUFFER_SIZE Zeichen.
unsigned char dataBuffer[BUFFER_SIZE];
uint8_t counter;

// Trennt die Verbindung zum Host für ca. 25 ms und meldet sich danach neu an
void usbForceDisconnect()
{
	// Re-enumeration forsieren während die Interrupts noch ausgeschalten sind
	usbDeviceDisconnect();

	// Ca. 250 ms warten
	uchar i = 0;
	while(--i)
	{
		wdt_reset();
		_delay_ms(1);
	}

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
		// Datenformat: [Sensordaten]*BUFFER_SIZE
		return CUSTOM_RQ_DATA_LEN;
	}
	// Im allgemeinen Fall gibt es diese Nachricht gar nicht. Wir geben deshalb nichts zurück.
	
	return 0;
}
