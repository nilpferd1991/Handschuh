#ifndef CONSOLE_H
#define CONSOLE_H

#include <ncurses.h>
#include <sstream>
#include <string>

using namespace std;

class Console
{
public:
	Console() { }
	~Console() { }
	
	static void initScreen();
	static void endScreen();
	static void refreshScreen();
	
	static void initKeystop();
	static void endKeystop();
	static bool keystop();
	
	static string currentEntry;
	static char* unfinishedEntry;
	static int currPos;
	static bool newString;
		
	static void newLine();
	static string getCom();
	static void deleteCom();
	static const char* getPar(int number);
	
	static void printBinaer(int zahl);
};

#endif
