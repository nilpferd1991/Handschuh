#include "TWI.h"
#include <avr/interrupt.h>

#define DEBUG 0

const uint8_t MAX_SENSOR_NUMBER = 1;

/// Number of sensors read so far. Increased by a new data packet.
uint8_t sensorRead = 0;
/// Status bit to check for second start (get the data)
volatile uint8_t status = 2;
/// The status of the current running sensor
uint8_t sensorNumber = 0;


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
	TWSR = 0;
	TWBR = ((F_CPU/100000L) - 16)/2;
}

/** 
 * twiPoll
 * Start a new data aquisition if possible
 */
void twiPoll() {
	if(status == 1) {
		twiSendReadAddress();
	}
	else if (status == 2) {
		twiSendWriteAddress();
	}
}

/**
 * twiGetData
 * Start a new process
 */
twiGetData() {
	 status = 2;
}

void twiSendWriteAddress() {
	cli();
	messageData[0] = 1;
	status = 0;
	TWI_START_NO_INT;
	uint8_t counter = 0;
	while(!(TWCR & (1 << TWINT)));
	TWDR = SLA_W;
	TWI_SEND;
	sei();
}

void twiSendReadAddress() {
	cli();
	messageData[0] = 4;
	status = 0;
	TWI_START_NO_INT;
	while(!(TWCR & (1 << TWINT)));
	TWDR = SLA_R;
	TWI_SEND;
	sei();
}

/**
 * ISR(TWI_vect)
 * Is executed whenever a new TWI signal comes from the slave.
 */
ISR(TWI_vect) {
	switch (TWSR & 0xF8) {
	// Write-Adresse angenommen. Nun senden wir den Offset 0x02
	case TW_MT_SLA_ACK:
		messageData[0] = 2;
		TWDR = 0x02;
		TWI_SEND;
		break;

	// Das Offset wurde angenommen. Nun können wir die Daten lesen. Also senden wir Start:
	case TW_MT_DATA_ACK:
		messageData[0] = 3;
		TWI_STOP;
		status = 1;
		break;

	// Die Read-Adresse wurde angenommen. Nun können wir Daten abholen:
	case TW_MR_SLA_ACK:
		messageData[0] = 5;
		sensorRead = 0;
		TWI_RECV_ACK;
		break;

	// Neue Daten sind angekommen. Wir rufen sie ab und fordern evtl neue an.
	case TW_MR_DATA_ACK:
		if(sensorRead < 7) {
			sensorData[sensorRead++] = TWDR;
			TWI_RECV_ACK;
		}
		else {
			messageData[0] = 6;
			TWI_RECV_NACK;
		}
		break;

	// Das letzte Datenpaket ist angekommen.
	case TW_MR_DATA_NACK:
		messageData[0] = 7;
		sensorRead = 0;
		TWI_STOP;
		while(TWCR & (1<<TWSTO));
		break;
		
	// Scheinbar ist etwas falsch gelaufen. Wir brechen ab.
	default:
		sensorRead = 0;
		status = 0;

		messageData[3] = TWSR & 0xF8;
		messageData[4] = 9;
		// Start/Stop senden
		TWI_START_NO_INT;
		break;
	}
}

/**
 * twiToggle
 * Toggle between the sensors. Turn on the sensors on PB0, PB1, etc.
 */
twiToggle() {
	sensorNumber++;
	if(sensorNumber > MAX_SENSOR_NUMBER) {
		sensorNumber = 0;
	}
	PORTB = (1 << sensorNumber);
}