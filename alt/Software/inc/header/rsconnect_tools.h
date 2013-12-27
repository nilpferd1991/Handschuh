#ifndef RSCONNECT_TOOLSH
#define RSCONNECT_TOOLSH

#include "rsconnect.h"
#include "console.h"
#include <stdio.h>

namespace tools
{
	extern long T_SELECT;
	extern long T_SETUP;
	extern long T_CLK;
	
	extern RSConnect _con;
	
	void RSpoweron();
	void RSpoweroff();
	void RSselect();
	void RStick();
	void RStickOnce();
	void RStickOnceOut();	
	void RSselectOnce();
	void RSselectOnceOut();
	bool RSread();
	
	void wait(unsigned long nanosec);
}

namespace Mfunc
{
	void MprintStatus();
	void Mread();
	void Mpaint();
	void MfileOpen(const char*);
	void MfileClose();
	void MfileWrite();
	
	extern int Mfcounter;
	extern clock_t Mstart;
	
	extern FILE* MfilePointer;
}


#endif
