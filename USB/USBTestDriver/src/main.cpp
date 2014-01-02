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

int main(int argc, char **argv)
{
	// Puffer für die Datenverarbeitung
	uint8_t * buffer;

	// USB-Schnittstelle initialisieren und mit Handschuh verbinden
	usbInit();

//	// Echo-Test
//	buffer = new uint8_t[CUSTOM_RQ_ECHO_LEN];
//	for(int i = 0; i < 10; i++)
//	{
//		usbRead(CUSTOM_RQ_ECHO, i, buffer, CUSTOM_RQ_ECHO_LEN);
//		printf("Echo erhielt:");
//		for(int j = 0; j < CUSTOM_RQ_ECHO_LEN; j++)
//		{
//			printf("%d ", buffer[j]);
//		}
//		printf("\n");
//	}

	// Data-Test
	buffer = new uint8_t[CUSTOM_RQ_DATA_LEN];

	for(;;) {
		usbRead(CUSTOM_RQ_DATA, 0, buffer, CUSTOM_RQ_DATA_LEN);
//		for(int i = 0; i < CUSTOM_RQ_DATA_LEN; i++) {
//			printf(" %x ", buffer[i]);
//		}
//		printf("\n");


		int_least16_t x,y,z;
		x = ((int_least8_t)buffer[1] << 2 | (int_least8_t)(buffer[0] >> 6));
		y = ((int_least8_t)buffer[3] << 2 | (int_least8_t)(buffer[2] >> 6));
		z = ((int_least8_t)buffer[5] << 2 | (int_least8_t)(buffer[4] >> 6));

		printf("%d %d %d", x,y,z);


		printf("\n");
	}

	usbClose();
	return 0;
}
