//============================================================================
// Name        : main.cpp
// Author      : Nils Braun
// Version     : 1
// Description : Treiber für die Verbindung zum ATmega16 über USB-Schnittstelle
//============================================================================


#include "driver.h"

#include <ncurses.h>

int counter = 0;
int status = 0;

#include <sys/time.h>
#include <ctime>

#define min(x,y) (x > y ? y : x)
#define max(x,y) (x < y ? y : x)

void setStatus(int_least16_t x, int_least16_t y, int_least16_t z, clock_t start, clock_t end) {

	// Daten zeichnen
	move( 10, 0);
	clrtoeol();
	printw("%d %d %d", x, y, z);

	move( 11, 0); clrtoeol();
	move( 12, 0); clrtoeol();
	move( 13, 0); clrtoeol();

	int maxx = getmaxx(stdscr);

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
	
	double usec =  (double) (end-start) / CLOCKS_PER_SEC * 1000000.0;
	move(15, 0);
	printw("computation time for %d samples: %f. computation time for 1 sample: %f\n", driver::MEASURE_CYCLES, usec, usec / driver::MEASURE_CYCLES);
	printw("Number of samples per second: %ld", (long int)(1/usec * 1000L));
	
	refresh();
}

void nc_init() {
	// Ncurses - Init
	initscr();
	noecho();	// Die Eingaben nicht ausgeben
	raw();		// Alle Eingaben abfangen
	nodelay(stdscr, TRUE); // Nicht auf Eingaben warten
	printw("Welcome to the control programm for the handglove!\n");

	// Startnachricht
	printw("Please go to state zero.\n");
	refresh();
}

int main(int argc, char **argv)
{
	driver glove = driver();

	// Initialize ncurses
	nc_init();
	
	FILE * file = fopen("output.dat", "w");

	for(int key;;) {

		// User hat eine Taste gedrückt
		if ((key = getch()) != ERR) {
			break;
		}

		// Daten abrufen
		int_least16_t x(0), y(0), z(0);

		clock_t start = clock();
		glove.catch_data(x, y, z);
	  clock_t end = clock();
		
		fprintf(file, "%d %d %d\n", x, y, z);
		
		setStatus(x, y, z, start, end);
	}

	usbClose();
	fclose(file);
	endwin();
	return 0;
}
