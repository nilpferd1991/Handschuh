#ifndef RSCONNECT_H
#define RSCONNECT_H

#include <sys/io.h>
#include <string.h>
#include <ncurses.h>
#include <iostream>
#include "console.h"
#include <time.h>

#define P_DTR  	1
#define P_RTS 	2
#define P_TxD	64
#define P_CTS	16
#define P_RI	64
#define P_DSR 	160

class RSConnect
{
private:	
	int status, output, input, power;
	const int port;

public:	
	RSConnect(const int _port);
	~RSConnect();
		
	int initSerialPort();
			
	// Get Pins
	bool getDSR();
	bool getCTS();
	bool getRI();
	bool getRxD();

	// Set Pins
	void setRTS();
	void unsetRTS();
	void setDTR();
	void unsetDTR();
	void setTxD();
	void unsetTxD();
	void unsetAll();
	
	// Printing-Functions
	void printHello();
	void printStatus();
};

#endif
