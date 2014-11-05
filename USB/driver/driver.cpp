#include "driver.h"

void driver::catch_data() {
	int_least16_t temp_1 = 0, temp_2 = 0, temp_3 = 0;
	
	uint8_t * buffer = new uint8_t[CUSTOM_RQ_DATA_LEN];
	
	for(int i = 0; i < MEASURE_CYCLES; i++) {
		usbRead(CUSTOM_RQ_DATA, 0, buffer, CUSTOM_RQ_DATA_LEN);

		temp_1 += ((int_least8_t)buffer[1] << 2 | (int_least8_t)(buffer[0] >> 6));
		temp_2 += ((int_least8_t)buffer[3] << 2 | (int_least8_t)(buffer[2] >> 6));
		temp_3 += ((int_least8_t)buffer[5] << 2 | (int_least8_t)(buffer[4] >> 6));

	}

	m_x = - temp_2/MEASURE_CYCLES;
	m_y = temp_1/MEASURE_CYCLES;
	m_z = temp_3/MEASURE_CYCLES;

}
