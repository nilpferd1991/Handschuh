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

#include <sys/time.h>

int main(int argc, char **argv)
{
	// Puffer für die Datenverarbeitung
	uint8_t * buffer;

	// USB-Schnittstelle initialisieren und mit Handschuh verbinden
	usbInit();

	// Data-Test
	buffer = new uint8_t[CUSTOM_RQ_DATA_LEN];

	struct timeval begin, end;
	long seconds, useconds;

	for(;;) {


		// Daten abrufen

		if (gettimeofday(&begin,(struct timezone *)0)) {
			fprintf(stderr, "can not get time\n");
		    exit(1);
		}

		for(int i = 0; i < 1000; i++) {
			usbRead(CUSTOM_RQ_DATA, 0, buffer, 16);

		}

		if (gettimeofday(&end,(struct timezone *)0)) {
			fprintf(stderr, "can not get time\n");
			exit(1);
		}

	  printf("begin:\t\t\t\t%ld sec %ld usec\n", begin.tv_sec, begin.tv_usec);
	  printf("end:\t\t\t\t%ld sec %ld usec\n", end.tv_sec, end.tv_usec);
	  seconds = end.tv_sec - begin.tv_sec;
	  useconds = end.tv_usec - begin.tv_usec;
	  if(useconds < 0) {
			useconds += 1000000;
			seconds--;
	  }

	  printf("Dauer eines Abrufs:\t\t%ld usec\n", (seconds*1000000L +  useconds)/1000L);
	  printf("Abrufe pro Sekunde:\t\t%lg\n\n", 1000000/(double)((seconds*1000000L +  useconds)/1000L));
	}

	usbClose();

	return 0;
}
