/* Name: main.c
 * Project: Handschuh Controller - die Firmware für den Handschuh
 * Author: Nils Braun
 */


// TESTING


#include <avr/io.h>
#include <avr/wdt.h>


int main(void)
{
	// Watchdog auf 1 Sekunde setzen, um bei einem Fehler sofort neuzustarten
	wdt_enable(WDTO_1S);

	DDRB = 0xFF;
	PORTB = 0x00;

	// USB: Nachrichtenschleife ausführen, watchdog zurücksetzen, USB-Nachrichten abrufen,
	// I2C-Nachrichten abrufen
	while(1)
	{
		wdt_reset();
	}

	return 0;
}
