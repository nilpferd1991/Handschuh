#ifndef TWI_H_
#define TWI_H_

#include <util/twi.h>
#include "usb.h"

// I2C-Adresse des Beschleunigungssensors
// für Schreibe- und Lesevorgänge
#define SLA_W 0x70
#define SLA_R 0x71

// I2C-Funktionen
inline int TW_SendStart();
inline int TW_SendAdress(uint8_t adress);
inline int TW_SendData(uint8_t data);
inline int TW_SendStop();
int getData();

#endif
