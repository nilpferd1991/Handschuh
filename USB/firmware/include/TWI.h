#pragma once

#include <util/twi.h>
#include "usb.h"

// I2C-Adresse des Beschleunigungssensors
// für Schreibe- und Lesevorgänge
#define SLA_W 0x70
#define SLA_R 0x71

#define TWI_START_NO_INT 		TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
#define TWI_SEND  					TWCR = (1 << TWINT)								 | (1 << TWEN) | (1 << TWIE);
#define TWI_RECV_ACK				TWCR = (1 << TWINT)								 | (1 << TWEN) | (1 << TWIE)							| (1 << TWEA);
#define TWI_RECV_NACK				TWCR = (1 << TWINT)								 | (1 << TWEN) | (1 << TWIE);
#define TWI_STOP  					TWCR = (1 << TWINT)								 | (1 << TWEN) 							| (1 << TWSTO);

void twiPoll();
void twiInit();
void twiToggle();
void twiGetData();
void twiSendWriteAddress();
void twiSendReadAddress();

extern uint16_t data_cycles_gathered;
extern uint16_t data_1, data_2, data_3;
