#include "rsconnect.h"

bool RSConnect::getDSR()
{
	return(inb(input) & P_DSR);
}

bool RSConnect::getCTS()
{
	return(inb(input) & P_CTS);
}

bool RSConnect::getRI()
{
	return(inb(input) & P_RI);
}

void RSConnect::setRTS()
{
	status |= P_RTS;
	outb(status, output);
}

void RSConnect::unsetRTS()
{
	status &= ~P_RTS;
	outb(status, output);
}

void RSConnect::setDTR()
{
	status |= P_DTR;
	outb(status, output);
}

void RSConnect::unsetDTR()
{
	status &= ~P_DTR;
	outb(status, output);
}

void RSConnect::setTxD()
{
	outb(P_TxD, power);
}

void RSConnect::unsetTxD()
{
	outb(0, power);
}


void RSConnect::unsetAll()
{
	unsetTxD();
	unsetRTS();
	unsetDTR();
}
