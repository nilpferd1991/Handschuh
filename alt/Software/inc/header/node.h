#ifndef NODE_H
#define NODE_H

#include "console.h"
#include <stdlib.h>
#include <math.h>


template <int maxx, int maxy>
class Node;

template <int maxx, int maxy>
class NodeList 
{
	friend class Node<maxx,maxy>;

private:
	Node<maxx, maxy>* _pointer;
	NodeList<maxx, maxy>* _next;
	double _factor;

public:
	
	NodeList(Node<maxx, maxy>* pointer, double factor)
	{
		_pointer = pointer;
		_next = 0;
		_factor = factor;
	}
	
	void addFactor(int step, double add)
	{
		if(step == 0)
			_factor += add;
		else
		{
			_next->addFactor(step - 1, add);
		}
	}
	
	double getFactor(int step)
	{
		if(step == 0)
			return _factor;
		else
		{
			return (_next->getFactor(step - 1));
		}
	}
	
};

template <int maxx, int maxy>
class Node
{	
private:
	double _value;
	int _name;
	
	static double*** _grads;
	static double** _inputs;
	static double** _outputs;
	
	static void calcValues();
	static void calcGradient();
	
public:	


	static Node<maxx, maxy>** _nodeList;

	NodeList<maxx, maxy>* _input;
	
	static int _maxinput;
	static double _h;
	static double _epsilon;
	
	Node()
	{
		_name = 0;
		_value = 0;
		_input = NULL;
	}
	
	void calcValue();
	void addInput(Node* input, double factor = (double)rand() / RAND_MAX);
	void setName(int name) { _name = name; }
	void setValue(double value) { _value = value; }
	double getValue() { return _value; }
	int getName() { return _name; }
			
	static void setInput(double** inputs) { _inputs = inputs; }
	static void setOutput(double** outputs) { _outputs = outputs; }
	static void initNodeList();
	static double calcError();
	static void applyGradient();
	
	static void test(double* inputs);
};

template <int maxx, int maxy>
void Node<maxx,maxy>::calcValue()
{
	_value = 0;
	for(NodeList<maxx,maxy>* _currpointer = _input; _currpointer != 0; _currpointer = _currpointer->_next)
	{
		_value += _currpointer->_pointer->_value * _currpointer->_factor;
	}
	_value = _value < 0 ? 0 : (_value > 1 ? 1 : _value);
}

template <int maxx, int maxy>	
void Node<maxx,maxy>::addInput(Node<maxx,maxy>* input, double factor)
{
	if(_input == 0)
	{
		_input = new NodeList<maxx,maxy>(input, factor);		
	}
	else
	{
		for(NodeList<maxx,maxy>* _currpointer = _input; _currpointer != 0; _currpointer = _currpointer->_next)
		{
			if(_currpointer->_next == 0)
			{
				_currpointer->_next = new NodeList<maxx,maxy>(input, factor);
				break;
			}
		}
	}
}

#include "node.h"

template <int maxx, int maxy>
Node<maxx,maxy>** Node<maxx,maxy>::_nodeList;

template <int maxx, int maxy>
double Node<maxx,maxy>::_h = 		0;

template <int maxx, int maxy>
double Node<maxx,maxy>::_epsilon = 	0;

template <int maxx, int maxy>
double*** Node<maxx,maxy>::_grads;

template <int maxx, int maxy>
int Node<maxx,maxy>::_maxinput =		0;

template <int maxx, int maxy>
double** Node<maxx,maxy>::_inputs;

template <int maxx, int maxy>
double** Node<maxx,maxy>::_outputs;


template <int maxx, int maxy>
// Initialisiert die Baumstruktur der Nervenzellen und füllt die Gewichte mit zufälligen Werten
void Node<maxx,maxy>::initNodeList()
{
	_nodeList = new Node<maxx,maxy>*[maxx];
	for(int i = 0; i < maxx; i++)
	{
		_nodeList[i] = new Node<maxx,maxy>[maxy];
		for(int j = 0; j < maxy; j++)
		{
			_nodeList[i][j].setName( i * maxy + (j + 1) );
			
			if(i > 0)
			{
				for(int jj = 0; jj < maxy; jj++)
				{
					_nodeList[i][j].addInput(&_nodeList[i - 1][jj]);
				}
			}
		}
	}
}


template <int maxx, int maxy>
// Berechnet für alle Ebenen den Wert
void Node<maxx,maxy>::calcValues()
{
	for(int i = 1; i < maxx; i++)
	{
		for(int j = 0; j < maxy; j++)
		{
			_nodeList[i][j].calcValue();
		}
	}
}

template <int maxx, int maxy>
// Berechnet den Fehler bei gegebenem Input und Output
double Node<maxx,maxy>::calcError()
{
	double _error = 0;
	
	for (int c = 0; c < _maxinput; c++)
	{
		// Input setzen
		for (int i = 0; i < maxy; i++)
		{
			_nodeList[0][i].setValue(_inputs[c][i]);
		}
	
		calcValues();
	
		for (int i = 0; i < maxy; i++)
		{
			double _currerror = _nodeList[maxx - 1][i].getValue() - _outputs[c][i];
			_error += _currerror * _currerror;
		}
	
	}
	
	return _error;
}

template <int maxx, int maxy>
// Berechnet den Gradienten für alle Ein- und Ausgaben
void Node<maxx,maxy>::calcGradient()
{	
	_grads = new double**[maxx - 1];
	
	for(int i = 1; i < maxx; i++)
	{
		_grads[i] = new double*[maxy];
		for(int j = 0; j < maxy; j++)
		{
			_grads[i][j] = new double[maxy];
			for(int jj = 0; jj < maxy; jj++)
			{	
				double _grad = 0;
				_nodeList[i][j]._input->addFactor(jj, _h);
				_grad = calcError();
				_nodeList[i][j]._input->addFactor(jj, -_h);
				_nodeList[i][j]._input->addFactor(jj, -_h);	
				_grad -= calcError();
				_grad /= (2 * _h);
			
				_grads[i][j][jj] = _epsilon * _grad;
				_nodeList[i][j]._input->addFactor(jj, _h);
			}
		}
	}
}

template <int maxx, int maxy>
// Wendet den berechneten Gradienten an
void Node<maxx,maxy>::applyGradient()
{
	calcGradient();
	for(int i = 1; i < maxx; i++)
	{
		for(int j = 0; j < maxy; j++)
		{
			for(int jj = 0; jj < maxy; jj++)
			{
				_nodeList[i][j]._input->addFactor(jj, -_grads[i][j][jj]);
			}
		}
	}
}

template <int maxx, int maxy>
// Testet das Netz
void Node<maxx,maxy>::test(double* inputs)
{
	// Input setzen
	for (int i = 0; i < maxy; i++)
	{
		_nodeList[0][i].setValue(inputs[i]);
	}

	calcValues();

	for (int i = 0; i < maxy; i++)
	{
		printw("%f\t", _nodeList[maxx - 1][i].getValue());
	}
}


#endif
