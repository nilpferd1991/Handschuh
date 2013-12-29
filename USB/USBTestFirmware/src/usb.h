/* Name: usb.h
 * Project: Handschuh Controller - die Firmware für den Handschuh
 * Author: Nils Braun
 */

/*
 * Hier werden die Funktionen zur Kommunikation des Handschuhs mit dem Host definiert.
 */

#ifndef USB_H_
#define USB_H_

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "certificate.h"
#include "usbdrv/usbdrv.h"
#include "TWI.h"

// Trennt den Handschuh absichtlich vom Host für ca. 250 ms und startet eine Reenumeration.
// Nach jedem Reset sollte eine Reenumerartion durchgeführt werden
void usbForceDisconnect();

// Sensordaten welche an den Host geschickt werden
// Die Definition muss extern sein, damit auch alle auf die selbe Speicherstelle zugreifen
// Die SensorDaten haben das folgende Format:
//		 Daten1	 	 Daten 2	 ....		 Daten 15		 Counter
// 		- 10 bit -	- 10 Bit -	 ....		- 10 Bit -		- 10 Bit -	|| 16 x 10 bit = 160 bit
//		Füllung															|| + 96 bit leer = 256 bit = 32 byte
extern uint8_t sensorData[BUFFER_SIZE];

// Counter zum Durchzählen der Sensordaten
extern uint8_t counter;

#endif /* USB_H_ */
