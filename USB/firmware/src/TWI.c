#include <TWI.h>
#include <logging.h>

#include <util/twi.h>
#include <usb.h>
#include <certificate.h>
#include <avr/interrupt.h>

#include <i2c/i2cmaster.h>

const uint8_t MAX_SENSOR_NUMBER = 1;

/// Number of sensors read so far. Increased by a new data packet.
uint8_t sensorRead = 0;
/// Status bit to check for second start (get the data)
volatile uint8_t status = 2;
/// In this buffer we write the incoming sensor data
uint8_t sensorData[BUFFER_SIZE];

/**
 * twiPoll
 * 
 * Function to read the sensor data from the sensor module via TWI.
 * This functions should be executed as freqently as possible. It checks for 
 * new messages in the TWI message buffer. If there are new messages, it tries to 
 * answer them. The control flow is as follows:
 * 
 * If there are new messages (TWCR & (1 << TWINT)) check for the type of message (TWSR & 0xF8).
 * 1. The main function sends the START signal
 * 2. TW_START: The start signal is acknowledged, so we send the adress to talk to (SLA_W)
 * 3. TW_MT_SLA_ACK: The address answered, so we send the offset (0x02) where we later want to read from
 * 4. TW_MT_DATA_ACK: The offset was acknowledged. So we want to start reading. We want to send the reading address (SLA_R), so we restart (status = 1)
 * 5. TW_REP_START: After restarting, we send the reading address (SLA_R).
 * 6. TW_MR_SLA_ACK: The reading address was acknowledged. We can start reading.
 * 7. TW_MR_DATA_ACK: We read until we have collected 6 data packets.
 * 8. TW_MR_DATA_NACK: We start again.
 */

/**
 * twiInit
 * Initialize the TWI module with the corresponding frequency
 */
void twiInit() {

	i2c_init();

	// I2C initialisieren
	// PC0, PC1: Eingänge mit Pullup = DATEN
	DDRC = 0;
	PORTC = (1 << 0) | (1 << 1);
}

/** 
 * twiPoll
 * Start a new data aquisition if possible
 */
void twiPoll() {
	if(status == 0) {
		twiSendWriteAddress();
	}
	else if (status == 1) {
		twiSendReadAddress();
	}
}

/**
 * twiGetData
 * Start a new process
 */
void twiGetData() {
	status = 0;
}

void twiSendWriteAddress() {
	cli();
	// Set the device into read mode
	i2c_start(SLA_W);

	// Send the data offset
	i2c_write(0x02);

	// Stop the transmission
	i2c_stop();
	status = 1;
	sei();
}

void twiSendReadAddress() {
	cli();
	i2c_start(SLA_R);
	for(sensorRead = 0; sensorRead < 6; sensorRead++) {
		sensorData[sensorRead] = i2c_readAck();
	}
	sensorData[sensorRead] = i2c_readNak();

	data_1 += ((int_least8_t)sensorData[1] << 2 | (int_least8_t)(sensorData[0] >> 6));
	data_2 += ((int_least8_t)sensorData[3] << 2 | (int_least8_t)(sensorData[2] >> 6));
	data_3 += ((int_least8_t)sensorData[5] << 2 | (int_least8_t)(sensorData[4] >> 6));

	data_cycles_gathered++;
	i2c_stop();

	status = 2;

	if(data_cycles_gathered < 8) {
		status = 0;
	}

	sei();
}

/**
 * twiToggle
 * Toggle between the sensors. Turn on the sensors on PB0, PB1, etc.
 * WHAT THE FUCK? THIS THROWS UP EVERYTHING!
 */
void twiToggle() {
	/*sensorNumber++;
	if(sensorNumber > MAX_SENSOR_NUMBER) {
		sensorNumber = 0;
	}
	PORTB = (1 << sensorNumber);*/
}
