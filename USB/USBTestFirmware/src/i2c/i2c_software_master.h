//###################################################################################
//	i2c_software_master.h
//	Autor: 			Artur Funk
//###################################################################################

#ifndef I2C_SOFTWARE_MASTER_H_
#define I2C_SOFTWARE_MASTER_H_

#include <avr/io.h>
#include <util/delay.h>

#include "i2c_config.h"

#define SDA_LESE_PIN 	PINC
#define SDA_LESE_BIT 	SDA

void 			i2c_master_init(void);
void			i2c_master_clk_toggle(void);
void 		 	i2c_master_start(void);
void 			i2c_master_stop(void);

unsigned char 		i2c_try_scl(unsigned char start_trials);
unsigned char  		i2c_master_write(unsigned char b);
unsigned char 		i2c_master_read(unsigned char a);

#endif
