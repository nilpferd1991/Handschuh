//============================================================================
// Name        : main.cpp
// Author      : Nils Braun
// Version     : 1
// Description : Treiber für die Verbindung zum ATmega16 über USB-Schnittstelle
//============================================================================


#include <stdlib.h>
#include <usb.h>

#include "certificate.h"
#include "usb.h"

int counter = 0;
int started = 0;

int main(int argc, char **argv)
{
	// Puffer für die Datenverarbeitung
	uint8_t * buffer;

	// USB-Schnittstelle initialisieren und mit Handschuh verbinden
	usbInit();

	// Data-Test
	buffer = new uint8_t[CUSTOM_RQ_DATA_LEN];

	for(;;) {
		usbRead(CUSTOM_RQ_DATA, 0, buffer, CUSTOM_RQ_DATA_LEN);
//		for(int i = 0; i < CUSTOM_RQ_DATA_LEN; i++) {
//			printf(" %x ", buffer[i]);
//		}
//		printf("\n");

		int_least16_t x,y,z;
		x = ((int_least8_t)buffer[1] << 2 | (int_least8_t)(buffer[0] >> 6)) - OFFSET_X;
		y = ((int_least8_t)buffer[3] << 2 | (int_least8_t)(buffer[2] >> 6)) - OFFSET_Y;
		z = ((int_least8_t)buffer[5] << 2 | (int_least8_t)(buffer[4] >> 6)) - OFFSET_Z;

		if(x*x < STEADY_STATE_THRESHOLD && y*y < STEADY_STATE_THRESHOLD && z*z < STEADY_STATE_THRESHOLD) {
			counter++;
		}
		else {
			counter = 0;
		}

		if(counter > STEADY_STATE_CYCLES) {
			printf("jetzt\n");
			started = 1;
			counter = 0;
		}

		if(started > 0) {
			printf("%d\t%d\t%d\n", x,y,z);
			started++;
		}
	}

	usbClose();
	return 0;
}
