#include "console.h"

string Console::currentEntry = "";
int Console::currPos = 0;
char* Console::unfinishedEntry = new char[128];
bool Console::newString = false;

void Console::initScreen()
{
	setlocale(LC_ALL, "de_DE.UTF-8");
	initscr();
	scrollok(stdscr, true);
	keypad(stdscr, true);
	refresh();
	
	*unfinishedEntry =  '\0';
	unfinishedEntry++;
	*unfinishedEntry =  '\0';
}

void Console::endScreen()
{
	printw("ok");
	endwin();
}

void Console::refreshScreen()
{
	printw("\r");
	refresh();
}

void Console::initKeystop()
{
	noecho();
	nodelay(stdscr, true);
}

void Console::endKeystop()
{	
	echo();
	nodelay(stdscr, false);
}

bool Console::keystop()
{
	return(getch() != -1);
}

void Console::newLine()
{
	move(4,0);
	addstr("\n > ");
	clrtoeol();
	addstr(unfinishedEntry);
	move(5, 3 + currPos);
		
	int _char = getch();
	if( _char != -1)
	{	
		if(_char == KEY_BACKSPACE)
		{
			for(int i = currPos; *(unfinishedEntry + i - 1) != '\0'; i++)
			{
				*(unfinishedEntry + i - 1) = *(unfinishedEntry + i);
			}
			currPos--;
			
		}
		else if(_char == 10)
		{
			newString = true;
			currentEntry = string(unfinishedEntry);
			currPos = 0;
			unfinishedEntry = new char[128];
			*unfinishedEntry = '\0';
			unfinishedEntry++;
			*unfinishedEntry =  '\0';
		}
		else if(_char == KEY_LEFT)
		{
			currPos--;
		}
		else if(_char == KEY_RIGHT)
		{
			if(!*(unfinishedEntry + currPos) == '\0')
				currPos++;
		}
		else
		{
			int _maxlenght = currPos;
			while (*(unfinishedEntry + _maxlenght - 1) != '\0')
			{
				_maxlenght++;
			}
			
			for(int i = _maxlenght + 1; i > currPos; i--)
			{
				*(unfinishedEntry + i) = *(unfinishedEntry + i - 1);
			}
			*(unfinishedEntry + currPos) = _char;
			currPos++;
		}
		
		if( currPos < 0)
			currPos = 0;
	}
	refresh();
	
		
}

string Console::getCom()
{
	newString = false;
	return (currentEntry.substr(0, currentEntry.find(" ")));
}

void Console::deleteCom()
{
	currentEntry = "";
}

const char* Console::getPar(int number)
{
	string parameters = currentEntry.substr(currentEntry.find(" ") + 1);
	istringstream iss(parameters);
	string sub;

	for(int i = 0; i < number; i++)
    	{
        	iss >> sub;
    	}
    	
	return(sub.c_str());
}
	
void Console::printBinaer(int zahl)
{
	int i = 0;
	int int_bit_size = sizeof(int) * 8;
	for(; i < int_bit_size; i++){
		printw("%s  ", ((zahl & (1 << i)) ? "1" : "0"));
	}
}
