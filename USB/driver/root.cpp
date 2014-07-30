#include "driver.h"
#include <iostream>
#include "TGraph.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include <cmath>
#include <stdio.h>

#define N 1000

#define CALIBRATION 0

const Int_t OFFSET_X = -2;
const Int_t OFFSET_Y = -39;
const Int_t OFFSET_Z = 252;

template <typename T>
class buffer {
	
public:
	buffer() : m_position(0), m_content() { };
	void add(T x) {
		m_content[m_position] = x;
		m_position = (m_position + 1) % N;
	}
	const T getElement(Int_t index) {
		return m_content[getRealIndex(index) % N];
	}
	
	const T mean() {
		T xi = 0;
		for(int i = 0; i < N; i++) {
			xi += m_content[i];
		}
		return xi/N;
	}
	
private:
	Int_t m_position;
	T m_content[N];
	
	Int_t getRealIndex(Int_t index) {
		Int_t realIndex;
		for(realIndex = index + m_position; realIndex < 0; realIndex += N);
		return realIndex % N;
	}
};

void root() {
	driver glove = driver();
	buffer<Double_t> x, y, z;
	
	FILE * file = fopen("output.dat", "w");
	
	if (CALIBRATION == 1) {
		std::cout << "Get ready..." << std::endl;
		sleep(1);
		std::cout << "Go!" << std::endl;
		for(Int_t i = 0; i < N; i++) {
			int_least16_t xi, yi, zi;
			glove.catch_data(xi, yi, zi);
			x.add(xi);
			y.add(yi);
			z.add(zi);
			if(i%100 == 0) {
				std::cout << "Stay!" << std::endl;
			}
		}
		std::cout << "OFFSET_X = " << x.mean() << " OFFSET_Y = " << y.mean() << " OFFSET_Z = " << z.mean() << std::endl;
		return;
	}
	
	Int_t stopCounter = 0;
	
	while(1) {
		int_least16_t xi, yi, zi;
		glove.catch_data(xi, yi, zi);
		x.add((xi - OFFSET_X)/512.0);
		y.add((yi - OFFSET_Y)/512.0);
		z.add((zi - OFFSET_Z)/512.0);
		fprintf(file, "%f %f %f\n", (xi - OFFSET_X)/512.0, (yi - OFFSET_Y)/512.0, (zi - OFFSET_Z)/512.0);
		
		Double_t maxX = 0, maxY = 0, maxZ = 0;
		for(int i = -300; i < 0; i++) {
			maxX = fabs(maxX) > fabs(x.getElement(i)) ? maxX : x.getElement(i);
			maxY = fabs(maxY) > fabs(y.getElement(i)) ? maxY : y.getElement(i);
			maxZ = fabs(maxZ) > fabs(z.getElement(i)) ? maxZ : z.getElement(i);
		}
				
		if(fabs(maxX) < 0.2 && maxY > 0.2 && stopCounter == 0) {
			std::cout << "Jetzt\r";
			stopCounter = 1;
		}
		else if(stopCounter != 0)
			std::cout << "Warte\r";
		else
			std::cout << "Nicht\r";
		
		if(stopCounter != 0)
			stopCounter = (stopCounter + 1) % 50;
	}
	
	fclose(file);
}

# ifndef __CINT__
int main()
{
	root();
	return 0;
}
# endif