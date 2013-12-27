//##########################################################################################
//			i2c_soft_master.c													 			|								 						|
//			Autor: 			Artur Funk	(a_funk80@yahoo.de)							 	|
// 			Last Change 	16.11.2007			   											|
//------------------------------------------------------------------------------------------
//##########################################################################################

#include "i2c_software_master.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void sda_low()
{
	I2C_PORT 		&=  ~(1<<SDA);    			// internen Pull-Up aus
	I2C_DDR 		|=   (1<<I2C_DDR_REG_BIT);  // Pin von SDA als Ausgang

}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void sda_high()
{
	I2C_DDR 		&= ~(1<<I2C_DDR_REG_BIT);  	// Pin von SDA als Eingang 
	I2C_PORT 		|=  (1<<SDA);    			// internen Pull-Up an 
} 
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
////////////////////////////////////////////////////////////////////////////////////////////

#define I2C_SCL_LOW		I2C_PORT &= ~(1 << SCL)	// Clock Low Output
#define I2C_SCL_HIGH	I2C_PORT |=  (1 << SCL)	// Clock High Output

#define I2C_SDA_LOW		sda_low()				// Daten Leitung Low
#define I2C_SDA_HIGH	sda_high()				// Data Wire High

#define	ACK				0						// Ack empfangen
#define	NACK			1						// Ack nicht empfangen

#define S_DELAY			_delay_loop_1(1);		// ca. 2.76µs H-Time
#define M_DELAY			_delay_us(1);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// Auf einen Moment warten, in dem der Bus frei ist
unsigned char i2c_try_scl(unsigned char start_trials)
{
	// 50 Mal kurz warten, wenn SCL auf LOW gehalten wird
	while((!SCL) && (start_trials <50))
	{
		start_trials++;
		S_DELAY;
	}
	if(!SCL) return 0 ; return 1;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

////////////////////////////////////////////////////////////////////////////////////////////
// I2C-Master Interface Standard Routinen
////////////////////////////////////////////////////////////////////////////////////////////
void i2c_master_init(void)
{
	I2C_DDR |=  (1<<SCL);  						// SCL Leitung als Ausgang definieren
		S_DELAY;
	I2C_SCL_HIGH;								// Clockleitung auf H setzen
	I2C_SDA_HIGH;								// Datenleitung auf H setzen
		S_DELAY;								// kurz warten
	i2c_master_stop();
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void i2c_master_clk_toggle(void)
{
	i2c_try_scl(0x0A);

	I2C_SCL_HIGH;
		S_DELAY;
		S_DELAY;
	I2C_SCL_LOW;
 		S_DELAY;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void i2c_master_start(void)
{
	// Übergang von H->L auf SDA, wobei SCL H ist.
	i2c_try_scl(0x0A);

	I2C_SDA_HIGH;			// SDA vorsichtshalber auf HIGH ziehen
	//	S_DELAY;
	I2C_SCL_HIGH;			// SCL auf High	
		S_DELAY;
	I2C_SDA_LOW;			// SDA auf Low ziehen. Ab jetzt mindestens 0.6 µs
		M_DELAY;
		M_DELAY;
		M_DELAY;
	I2C_SCL_LOW;			// SCL auf low
		S_DELAY;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void i2c_master_stop(void)		
{
	// Übergang von L->H auf SDA, wobei SCL H ist.
	i2c_try_scl(0x0A);
		S_DELAY;
	I2C_SDA_LOW;			// SDA vorsichtshalber auf LOW ziehen
		S_DELAY;			// Kurz abwarten
	I2C_SCL_HIGH;			// SCL auf High setzen
		S_DELAY;		
	I2C_SDA_HIGH;			// SDA von L auf H ziehen
		S_DELAY;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
unsigned char i2c_master_write(unsigned char b)
{	
	char i=0;
	for(i=0;i<8;i++)
	{
		if (b & 0x80)	
		{
			I2C_SDA_HIGH;
		}
		else
		{
			I2C_SDA_LOW;
		}
		b=b<<1;
		S_DELAY;
		i2c_master_clk_toggle();
	}
	// Auf ACK vom Slave Prüfen
	I2C_SDA_HIGH;
	i2c_master_clk_toggle();		                                                                                                                   
	if(SDA==1)	
	{
		return 0;
	}
	else
	{ 
		return 1;
	}	
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
unsigned char i2c_master_read(unsigned char a)
{	
	unsigned char i,c=0b00000000;
	I2C_SDA_HIGH;      

	for(i=0;i<8;i++)		
	{
		c=c<<1;				

		// Bitweise OR verknüpfen mit dem Eingang, MSB zuerst
		if(SDA_LESE_PIN & (1 << SDA_LESE_BIT)) c|=1;			
		i2c_master_clk_toggle();
	}

	// ACK oder NACK | ACK=0; NACK=1
	if(a==ACK)	{I2C_SDA_LOW;} else {I2C_SDA_HIGH;}  
	i2c_master_clk_toggle();
	return c;
}

