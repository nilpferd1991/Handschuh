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
int status = 0;

#define STATUS_STARTING 0
#define STATUS_STEADY_STATE 1
#define STATUS_AFTER_STEADY_STATE 2
#define STATUS_NOT_STEADY_STATE 3
#define STATUS_BEFORE_STEADY_STATE 4

int main(int argc, char **argv)
{
	// Puffer für die Datenverarbeitung
	uint8_t * buffer;

	// USB-Schnittstelle initialisieren und mit Handschuh verbinden
	usbInit();

	// Data-Test
	buffer = new uint8_t[CUSTOM_RQ_DATA_LEN];

	// Startnachricht
	printf("Bitte begeben Sie sich in Ausgangsstellung.\n");

	for(;;) {

		int_least16_t x,y,z;

		for(int i = 0; i < MEASURE_CYCLES; i++) {
			usbRead(CUSTOM_RQ_DATA, 0, buffer, CUSTOM_RQ_DATA_LEN);

			x += ((int_least8_t)buffer[1] << 2 | (int_least8_t)(buffer[0] >> 6)) - OFFSET_X;
			y += ((int_least8_t)buffer[3] << 2 | (int_least8_t)(buffer[2] >> 6)) - OFFSET_Y;
			z += ((int_least8_t)buffer[5] << 2 | (int_least8_t)(buffer[4] >> 6)) - OFFSET_Z;

		}

		x /= MEASURE_CYCLES;
		y /= MEASURE_CYCLES;
		z /= MEASURE_CYCLES;


		switch(status) {
		// Gerade gestartet
		case STATUS_STARTING:
			// Sind wir im Steady-State?
			if(x*x < STEADY_STATE_THRESHOLD && y*y < STEADY_STATE_THRESHOLD &&
					z*z < STEADY_STATE_THRESHOLD) {
				status = STATUS_STEADY_STATE;
				counter = 0;
				printf("Anfangsstellung halten...\n");
			}
			break;
		// Gerade im Steady-State
		case STATUS_STEADY_STATE:
			if(x*x < STEADY_STATE_THRESHOLD && y*y < STEADY_STATE_THRESHOLD &&
					z*z < STEADY_STATE_THRESHOLD) {
				counter++;
			}

			// Sind wir lange genug im Steady-State geblieben?
			if(counter > STEADY_STATE_CYCLES) {
				status = STATUS_AFTER_STEADY_STATE;
				counter = 0;
				printf("Anfangsstellung erkannt. Jetzt können Bewegungen ausgeführt werden.\n");
			}
			break;
		// Nach dem Steady-State
		case STATUS_AFTER_STEADY_STATE:
			if(!(x*x < STEADY_STATE_THRESHOLD && y*y < STEADY_STATE_THRESHOLD &&
					z*z < STEADY_STATE_THRESHOLD)) {
				status = STATUS_NOT_STEADY_STATE;
				printf("Bewegung erkannt.\n");
			}
			break;
		// Nach Steady-State mit Bewegung
		case STATUS_NOT_STEADY_STATE:

			if(x*x < STEADY_STATE_THRESHOLD && y*y < STEADY_STATE_THRESHOLD &&
					z*z < STEADY_STATE_THRESHOLD) {
				status = STATUS_BEFORE_STEADY_STATE;
				counter = 0;
				printf("Schon fertig mit der Bewegung?\n");
			}
			break;
		case STATUS_BEFORE_STEADY_STATE:

			if(x*x < STEADY_STATE_THRESHOLD && y*y < STEADY_STATE_THRESHOLD &&
					z*z < STEADY_STATE_THRESHOLD) {
				counter++;
			}
			else {
				printf("Nein.\n");
				status = STATUS_NOT_STEADY_STATE;
				break;
			}

			// Sind wir lange genug im Steady-State geblieben?
			if(counter > STEADY_STATE_CYCLES) {
				status = STATUS_AFTER_STEADY_STATE;
				printf("Ja.\n");
				counter = 0;
			}
			break;
		default:
			break;
		}
	}

	usbClose();
	return 0;
}
