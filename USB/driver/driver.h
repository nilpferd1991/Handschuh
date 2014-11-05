#ifndef __driver_h_included__
#define __driver_h_included__

#include <stdlib.h>
#include <usb.h>
#include <stdio.h>

#include "certificate.h"
#include "usb.h"

class driver {

public:
	driver() : m_x(0), m_y(0), m_z(0) { usbInit(); }
	static const int OFFSET_X = 0;
	static const int OFFSET_Y = 0;
	static const int OFFSET_Z = 0;
	static const int MEASURE_CYCLES = 10;

	/**
	* catch_data
	* Catch the data from the driver. Can only be executed if the device is instalized.
	*/
	void catch_data();
	void catch_data(int_least16_t & x, int_least16_t & y, int_least16_t & z) { 
		catch_data(); 
		x = m_x; 
		y = m_y;
		z = m_z;
	}

private:
	int_least16_t m_x, m_y, m_z;
};

#endif
