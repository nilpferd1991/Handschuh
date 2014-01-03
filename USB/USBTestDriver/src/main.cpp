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

#include <ncurses.h>

int counter = 0;
int status = 0;

#define STATUS_STARTING 0
#define STATUS_STEADY_STATE 1
#define STATUS_AFTER_STEADY_STATE 2
#define STATUS_NOT_STEADY_STATE 3
#define STATUS_BEFORE_STEADY_STATE 4


#define min(x,y) (x > y ? y : x)
#define max(x,y) (x < y ? y : x)

int main(int argc, char **argv)
{
	// Puffer für die Datenverarbeitung
	uint8_t * buffer;

	// USB-Schnittstelle initialisieren und mit Handschuh verbinden
	usbInit();

	// Data-Test
	buffer = new uint8_t[CUSTOM_RQ_DATA_LEN];

	// Ncurses - Init
	initscr();
	noecho();	// Die Eingaben nicht ausgeben
	raw();		// Alle Eingaben abfangen
	nodelay(stdscr, TRUE); // Nicht auf Eingaben warten
	printw("Welcome to the control programm for the handglove!\n");

	// Startnachricht
	printw("Please go to state zero.\n");
	refresh();

	int key;
	int maxx = getmaxx(stdscr);
	int maxy = getmaxy(stdscr);

	for(;;) {

		// User hat eine Taste gedrückt
		if ((key = getch()) != ERR) {
			usbClose();

			endwin();
			return 0;
		}

		// Daten abrufen
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

		// Daten zeichnen
		move( 10, 0);
		clrtoeol();
		printw("%d %d %d", x, y, z);

		move( 11, 0); clrtoeol();
		move( 12, 0); clrtoeol();
		move( 13, 0); clrtoeol();

		for(int i = 0; i < maxx; i++) {
			if(i > min(maxx/2, x/10 + maxx/2) && i < max(maxx/2, x/10 + maxx/2)) {
				move( 11, i);
				addch('x');
			}
			if(i > min(maxx/2, y/10 + maxx/2) && i < max(maxx/2, y/10 + maxx/2)) {
				move( 12, i);
				addch('y');
			}
			if(i > min(maxx/2, z/10 + maxx/2) && i < max(maxx/2, z/10 + maxx/2)) {
				move( 13, i);
				addch('z');
			}
		}


		// Schrift ausgeben

		move(4,0);
		printw("state zero: false\n");

		move(3,0);
		switch(status) {
		// Gerade gestartet
		case STATUS_STARTING:
			move(4,0);
			printw("state zero: maybe\n");
			// Sind wir im Steady-State?
			if(x*x < STEADY_STATE_THRESHOLD && y*y < STEADY_STATE_THRESHOLD &&
					z*z < STEADY_STATE_THRESHOLD) {
				status = STATUS_STEADY_STATE;
				counter = 0;
			}
			break;
		// Gerade im Steady-State
		case STATUS_STEADY_STATE:
			move(4,0);
			printw("state zero: maybe\n");

			if(x*x < STEADY_STATE_THRESHOLD && y*y < STEADY_STATE_THRESHOLD &&
					z*z < STEADY_STATE_THRESHOLD) {
				counter++;
			}

			// Sind wir lange genug im Steady-State geblieben?
			if(counter > STEADY_STATE_CYCLES) {
				status = STATUS_AFTER_STEADY_STATE;
				counter = 0;
			}
			break;
		// Nach dem Steady-State
		case STATUS_AFTER_STEADY_STATE:
			move(4,0);
			printw("state zero: true\n");

			if(!(x*x < STEADY_STATE_THRESHOLD && y*y < STEADY_STATE_THRESHOLD &&
					z*z < STEADY_STATE_THRESHOLD)) {
				status = STATUS_NOT_STEADY_STATE;
			}
			break;
		// Nach Steady-State mit Bewegung
		case STATUS_NOT_STEADY_STATE:
			move(4,0);
			printw("state zero: false\n");

			if(x*x < STEADY_STATE_THRESHOLD && y*y < STEADY_STATE_THRESHOLD &&
					z*z < STEADY_STATE_THRESHOLD) {
				status = STATUS_BEFORE_STEADY_STATE;
				counter = 0;
			}
			break;
		case STATUS_BEFORE_STEADY_STATE:

			move(4,0);
			printw("state zero: maybe\n");

			if(x*x < STEADY_STATE_THRESHOLD && y*y < STEADY_STATE_THRESHOLD &&
					z*z < STEADY_STATE_THRESHOLD) {
				counter++;
			}
			else {
				status = STATUS_NOT_STEADY_STATE;
				break;
			}

			// Sind wir lange genug im Steady-State geblieben?
			if(counter > STEADY_STATE_CYCLES) {
				status = STATUS_AFTER_STEADY_STATE;
				counter = 0;
			}
			break;
		default:
			break;
		}

		refresh();
	}

	usbClose();

	endwin();
	return 0;
}
