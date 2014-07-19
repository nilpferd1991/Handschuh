/* Name: usb.c
 * Project: Handschuh Controller - die Firmware für den Handschuh
 * Author: Nils Braun
 */

#include "usb.h"
#include <util/delay.h>

// Deklaration der externen Daten
uint8_t sensorData[BUFFER_SIZE];
uint8_t messageData[BUFFER_SIZE];


static unsigned char bytesRemaining, currentPosition;


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
}



// Diese Funktion wird immer dann aufgerufen, wenn eine neue Nachricht (vom Host) eintrifft
// setupData enthält die Kenngrößen der Nachricht.
USB_PUBLIC usbMsgLen_t usbFunctionSetup(uchar setupData[8])
{
	// Setup-Nachricht vom Host ins richtige Format umwandeln
	usbRequest_t *request = (void*)setupData;

	// request->wLength enthält die geforderte Länge
	// request->wValue enthält den übergebenen Wert

	// Der RequestCode des Requests entscheidet über die Art der Programmausführung

	// DATA
	// Gebe die Sensordaten an den Host zurück
	if(request->bRequest == CUSTOM_RQ_DATA)
	{
		usbMsgPtr = sensorData;
		twiGetData();
		return CUSTOM_RQ_DATA_LEN;
	}
	
	// LOG
	// Gebe die Logdaten an den Host zurück
	else if(request->bRequest == CUSTOM_RQ_LOG)
	{
		usbMsgPtr = messageData;
		return CUSTOM_RQ_LOG_LEN;
	}
	
	// TOGGLE
	// Wechsle zwischen den einzelnen Sensoren
	else if(request->bRequest == CUSTOM_RQ_TOGGLE)
	{
		twiToggle();
		return CUSTOM_RQ_TOGGLE_LEN;
	}

	// Im allgemeinen Fall gibt es diese Nachricht gar nicht. Wir geben deshalb nichts zurück.
	return 0;
}