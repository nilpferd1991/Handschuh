#include "rsconnect_tools.h"

namespace Mfunc
{
	int Mfcounter = 0;
	clock_t Mstart = 0;
	FILE * MfilePointer = 0;
	
	
	void MprintStatus()
	{
		mvaddstr(0, 0, "Statusdiagram of RS-232:");
		clrtoeol();
		if(MfilePointer == NULL)
		{
			mvaddstr(0, 50, "No file opened");	
		}
		else
		{
			mvaddstr(0, 50, "A file opened");	
		}
		move(2,0);
		tools::_con.printStatus(); 
	}
	
	void Mread()
	{
		mvaddstr(6, 0, "Datatable:");
		tools::RSpoweron();
		
		for(int datarow = 0; datarow < 16; datarow++)
		{
			if(datarow == 0)
			{
				move(7, 0);
				addstr("n/s");
				move(8, 0);
				if(Mfcounter % 100 == 0)
				{
					printw("%5.2f",  100 * (double)CLOCKS_PER_SEC / (double)(clock() - Mstart));
					Mfcounter = 0;
					Mstart = clock();
				}
				Mfcounter++;
			}
			else
			{
				tools::RSselect();
				int _value = 0;
				for(int i = 0; i < 8; i++)
				{
					_value = _value << 1;
					_value += tools::RSread() ? 1 : 0;
					tools::RStick();
				}
				move(7, datarow * 8);
				printw("%d", datarow);
				move(8, datarow * 8);
				printw("%d\t\t", _value);
			}
		}
	}
	
	void Mpaint()
	{
		mvaddstr(6, 0, "Datagraph:");
		tools::RSpoweron();
		
		for(int datarow = 0; datarow < 16; datarow++)
		{
			if(datarow == 0)
			{
				move(7, 0);
				if(Mfcounter % 100 == 0)
				{
					printw("%5.2f",  100 * (double)CLOCKS_PER_SEC / (double)(clock() - Mstart));
					Mfcounter = 0;
					Mstart = clock();
				}
				Mfcounter++;
				
				for(int i = 0; i <= 4; i++)
				{
					move(8, 32 * i + 3);
					printw("%d", i*64);
				}
			}
			else
			{
				move(8 + datarow, 0);
				printw("%d", datarow);
				clrtoeol();
				
				tools::RSselect();
				int _value = 0;
				for(int i = 0; i < 8; i++)
				{
					_value = _value << 1;
					_value += tools::RSread() ? 1 : 0;
					tools::RStick();
				}
				
				for (int i = 0; 2*i < _value; i++)
				{
					move(8 + datarow, 3 + i);
					addch('=');				
				}
			}
		}
	}
	
	void MfileOpen(const char* fileName)
	{
		MfilePointer = fopen(fileName, "a+");
		fputs("New Dataline:\n", MfilePointer);
		time_t t;
		time(&t);
		fprintf(MfilePointer, "%s\n", ctime(&t));
	}
	
	void MfileClose()
	{
		if(MfilePointer != NULL)
		{
			fclose(MfilePointer);
			MfilePointer = 0;
		}
	}
	
	void MfileWrite()
	{
		if(MfilePointer != NULL)
		{
			tools::RSpoweron();
		
			for(int datarow = 1; datarow < 16; datarow++)
			{
				tools::RSselect();
				int _value = 0;
				for(int i = 0; i < 8; i++)
				{
					_value = _value << 1;
					_value += tools::RSread() ? 1 : 0;
					tools::RStick();
				}
				fprintf(MfilePointer, "%d ", _value);
			}
			fputs("\n", MfilePointer);
		}
	}
}
