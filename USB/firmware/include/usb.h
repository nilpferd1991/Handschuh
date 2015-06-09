/* Name: usb.h
 * Project: Handschuh Controller - die Firmware für den Handschuh
 * Author: Nils Braun
 */

/*
 * Hier werden die Funktionen zur Kommunikation des Handschuhs mit dem Host definiert.
 */
#pragma once

// Trennt den Handschuh absichtlich vom Host für ca. 250 ms und startet eine Reenumeration.
// Nach jedem Reset sollte eine Reenumerartion durchgeführt werden
void usbForceDisconnect();
