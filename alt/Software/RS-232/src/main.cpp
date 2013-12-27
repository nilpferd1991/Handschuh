#include "rsconnect.h"
#include "rsconnect_tools.h"
#include "console.h"

using namespace tools;
using namespace Mfunc;

int main()
{
	bool _recording = false;


	Console::initScreen();

	RSpoweron();
	
	Console::initKeystop();
		
	while(1)
	{	
		MprintStatus();
		Console::newLine();
		
		string com = Console::getCom();
		
		if(com == "exit")
		{
			break;
		}
		else if(com == "data")
		{
			Mread();
		}
		else if(com == "tick")
		{
			RStickOnce();
		}
		else if(com == "tickOff")
		{
			RStickOnceOut();
		}
		else if(com == "select")
		{
			RSselectOnce();
		}
		else if(com == "selectOff")
		{
			RSselectOnceOut();
		}
		else if(com == "paint")
		{
			Mpaint();
		}
		else if(com == "open")
		{
			MfileOpen(Console::getPar(1));
			Console::deleteCom();
		}
		else if(com == "close")
		{
			MfileClose();
			Console::deleteCom();
		}
		else if(com == "write")
		{
			MfileWrite();
		}
		else if(com == "record")
		{
			mvaddstr(6,0, "Press a to start recording, d to stop.");
			if (*Console::unfinishedEntry == 'a')
			{
				Console::unfinishedEntry = new char[128];
				*Console::unfinishedEntry = '\0';
				Console::unfinishedEntry++;
				*Console::unfinishedEntry =  '\0';
				Console::currPos = 0;
				_recording = true;
			}
			else if (*Console::unfinishedEntry == 'd')
			{
				Console::unfinishedEntry = new char[128];
				*Console::unfinishedEntry = '\0';
				Console::unfinishedEntry++;
				*Console::unfinishedEntry =  '\0';
				Console::currPos = 0;
				_recording = false;
			}				
			if(_recording)
			{
				MfileWrite();
				mvaddstr(7,0, "recording");
			}
			else
			{	
				move(7,0);
				clrtoeol();	
			}
		}
		else if(com == "stop")
		{
			clear();
			Console::deleteCom();
		}
	}
	
	RSpoweroff();
	
	Console::endKeystop();
	Console::endScreen();
}
