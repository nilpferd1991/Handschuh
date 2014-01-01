#ifndef TWI_H_
#define TWI_H_

#include <util/twi.h>
#include "usb.h"

// I2C-Adresse des Beschleunigungssensors
// für Schreibe- und Lesevorgänge
#define SLA_W 0x70
#define SLA_R 0x71

int getData();

void twiPoll();

#endif
