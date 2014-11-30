#include "driver.h"

void driver::catch_data() {
	int_least16_t temp_1 = 0, temp_2 = 0, temp_3 = 0;
	
	uint8_t * buffer = new uint8_t[CUSTOM_RQ_DATA_LEN];
	
	int_least16_t counter = 0;

	while(counter < MEASURE_CYCLES) {
		usbRead(CUSTOM_RQ_DATA, 0, buffer, CUSTOM_RQ_DATA_LEN);

		if(buffer[6] != 0 or buffer[7] != 0) {
			temp_1 += buffer[0];
			temp_1 += buffer[1] << 8;
			temp_2 += buffer[2];
			temp_2 += buffer[3] << 8;
			temp_3 += buffer[4];
			temp_3 += buffer[5] << 8;
			counter += buffer[6];
			counter += buffer[7] << 8;
		}

	}

	if(counter != 0) {
		m_x = - temp_2/counter;
		m_y = temp_1/counter;
		m_z = temp_3/counter;
	}

}
