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

#endif /* USB_H_ */
