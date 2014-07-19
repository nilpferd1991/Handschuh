//============================================================================
// Name        : usb.h
// Author      : Nils Braun
// Version     : 1
// Description : Alle wichtigen USB-Funktionen, welche die Kommunikation,
//				die Initialisierung, das Senden und das Empfangen
//				von Daten auf einem ganz rudimentären Niveau ermöglichen
//============================================================================

#ifndef USB_H_C
#define USB_H_C

#include <usb.h>

#include "opendevice/opendevice.h"
#include "usbconfig.h"

// Handle für die Kommunikation mit dem Handschuh
extern usb_dev_handle * handle;

// Initialisiert die Schnittstelle und baut eine Verbindung zum Handschuh auf
void usbInit();

// Schließt die Verbindung und räumt auf
void usbClose();

// Sendet einen Request mit dem Code requestCode und dem Wert value an den Handschuh.
// Die Antwort kommt im buffer zurück. size gibt die erwartete Datenlänge an.
// Der Buffer muss mindestens size u_char-Daten enthalten können.
// Dient zur Datenabfrage.
void usbRead(const int requestCode, const int value, uint8_t * buffer, const int sizes);

// Sendet einen Request mit dem Code requestCode und dem Wert value an den Handschuh.
// Es wird keine Antwort erwartet.
// Dient zum Steuern des Handschuhs.
void usbWrite(const int requestCode, const int value);

#endif /* USB_H_C */
