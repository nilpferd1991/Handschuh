#include "rsconnect.h"

RSConnect::RSConnect(const int _port) : port(_port)
{
	status = 0;
	output = port + 4;
	input = port + 6;
	power = port + 3;
		
	initSerialPort();
}

RSConnect::~RSConnect()
{
	unsetAll();
}

int RSConnect::initSerialPort()
{    
	ioperm(port, 10, 1);
	unsetAll();

	return(0);
}


void RSConnect::printHello()
{
	printw("\nSerielle Schnittstelle \nÜberwachungscenter\n");
	printw("Öffne Verbindung an %s\n\n", port);
}

void RSConnect::printStatus()
{
	printw("DSR\tCTS\tRI\t");
	for(unsigned int i = 0; i < sizeof(int) * 8; i++)
	{
		printw("%d ", i);
		if( i < 10 ) printw(" ");
	}
	printw("\n");
		
	printw("%s", (getDSR() ? "1" : "0"));
	printw("\t");

	printw("%s", (getCTS() ? "1" : "0"));
	printw("\t");

	printw("%s", (getRI() ? "1" : "0"));
	printw("\t");
	
	Console::printBinaer(inb(input));
	
	printw("\n");
}

