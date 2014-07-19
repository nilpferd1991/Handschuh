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

	// ECHO
	if(request->bRequest == CUSTOM_RQ_ECHO)
	{
		// Puffer mit der Nachricht nach draußen. Sie enthält maximal BUFFER_SIZE Zeichen.
		unsigned char dataBuffer[BUFFER_SIZE];
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
		
		bytesRemaining = request->wLength.word;
		currentPosition = 0;
		// Datenformat: [Sensordaten]*BUFFER_SIZE
		return USB_NO_MSG;
	}
	
	// LOG
	// Gebe die Logdaten an den Host zurück
	else if(request->bRequest == CUSTOM_RQ_LOG)
	{
		usbMsgPtr = messageData;

		// Datenformat: BUFFER_SIZE
		return CUSTOM_RQ_LOG_LEN;
	}

	// Im allgemeinen Fall gibt es diese Nachricht gar nicht. Wir geben deshalb nichts zurück.
	return 0;
}

// NOT USED
USB_PUBLIC uchar usbFunctionRead(unsigned char *data, unsigned char len)
{
	unsigned char i;
	if(len > bytesRemaining)                // len is max chunk size
		len = bytesRemaining;               // send an incomplete chunk
	bytesRemaining -= len;
	
	status = 2;
	
	for(i = currentPosition; i < len; i++)
		data[i] = sensorData[i]; // copy the data to the buffer
	return len;                             // return real chunk size*/
}
