#include "node.h"
#include <string.h>

#define Nodes	Node<4,2>

int main()
{
// Initialisierungen
	double _error = 1;
	double _olderror = 0;
	Nodes::_h = 0.01;
	Nodes::_epsilon = 0.001;

// Input und Output auslesen

	FILE* DataPointer = fopen("data", "r+");
	FILE* FinalPointer = fopen("final", "w+");

	long lSize;
  	char * buffer, * _buffer;
  	size_t result;

 	if (DataPointer == NULL) {fputs ("Error while opening file.", stderr); exit (1);}

	fseek (DataPointer , 0 , SEEK_END);
	lSize = ftell(DataPointer);
	rewind (DataPointer);

	buffer = (char*) malloc (sizeof(char)*lSize);
	if (buffer == NULL) {fputs ("Memory error", stderr); exit(2);}

	result = fread (buffer, 1, lSize, DataPointer);
	if (result != lSize) {fputs ("Error while reading file.", stderr); exit(3);}

	_buffer = buffer;

	while(*_buffer != '\0')
	{
		for(int i = 0; _buffer[i] != '\n'; i++)
		{
			
		}
	}

	free (buffer);

	double** _inputs = new double*[4];
	_inputs[0] = new double[2];
	_inputs[1] = new double[2];
	_inputs[2] = new double[2];
	_inputs[3] = new double[2];
	_inputs[0][0] = 0;
	_inputs[0][1] = 0;
	_inputs[1][0] = 1;
	_inputs[1][1] = 1;
	_inputs[2][0] = 1;
	_inputs[2][1] = 0;
	_inputs[3][0] = 0;
	_inputs[3][1] = 1;
	
	double** _outputs = new double*[4];
	_outputs[0] = new double[2];
	_outputs[1] = new double[2];
	_outputs[2] = new double[2];
	_outputs[3] = new double[2];
	_outputs[0][0] = 0;
	_outputs[0][1] = 0;
	_outputs[1][0] = 0;
	_outputs[1][1] = 0;
	_outputs[2][0] = 1;
	_outputs[2][1] = 0;
	_outputs[3][0] = 1;
	_outputs[3][1] = 0;

// Schreibe und Lese Funktion für Daten
// Schreibe und Lese Funktion für Neuronale Netze

// END TODO
	
	Console::initScreen();
	Console::initKeystop();	
	
	Nodes::setInput(_inputs);
	Nodes::setOutput(_outputs);
	
	srand(time(NULL));
	
	while(_error != 0)
	{
		Nodes::initNodeList();
	
		while(1)
		{
			Nodes::applyGradient();
		
			_error = Nodes::calcError();
			if(_error == _olderror)
				break;
			_olderror = _error;
		}
	}	
	
	while(1)
	{		
		move(7,0);
		Nodes::test(_inputs[0]);
		move(8,0);
		Nodes::test(_inputs[1]);
		move(9,0);
		Nodes::test(_inputs[2]);
		move(10,0);
		Nodes::test(_inputs[3]);
	
		Console::newLine();
		
		string com = Console::getCom();
		
		if(com == "exit")
		{
			break;
		}
	}
	
	fclose(DataPointer);
	fclose(FinalPointer);
	
	Console::endKeystop();
	Console::endScreen();
}
