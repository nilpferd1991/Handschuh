#include "driver.h"

void driver::catch_data() {
	m_x = 0;
	m_y = 0;
	m_z = 0;
	
	uint8_t * buffer = new uint8_t[CUSTOM_RQ_DATA_LEN];
	
	for(int i = 0; i < MEASURE_CYCLES; i++) {
		usbRead(CUSTOM_RQ_DATA, 0, buffer, CUSTOM_RQ_DATA_LEN);

		m_x += ((int_least8_t)buffer[1] << 2 | (int_least8_t)(buffer[0] >> 6)) - OFFSET_X;
		m_y += ((int_least8_t)buffer[3] << 2 | (int_least8_t)(buffer[2] >> 6)) - OFFSET_Y;
		m_z += ((int_least8_t)buffer[5] << 2 | (int_least8_t)(buffer[4] >> 6)) - OFFSET_Z;

	}

	m_x /= MEASURE_CYCLES;
	m_y /= MEASURE_CYCLES;
	m_z /= MEASURE_CYCLES;

}