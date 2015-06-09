/* Name: certificate.c
 * Project: Handschuh Controller - die Firmware für den Handschuh
 * Author: Nils Braun
 */

/*
 * Hier werden alle möglichen RequestCodes für die Kommunikation zwischen Host und Handschuh definiert
 */

#pragma once

// Buffer-Größe eines Buffers in unsigned char abgespeichert
// Größe ist in Byte angegeben
#define BUFFER_SIZE 8

// Das Data-Kommando fragt die momentanen Daten vom Handschuh ab. Die Antwort ist ein BUFFER_SIZE-Byte-Puffer
// mit den momentanen Sensordaten
#define CUSTOM_RQ_DATA 				1
#define CUSTOM_RQ_DATA_LEN			BUFFER_SIZE

// Das Toggle-Kommando wechselt zwischen den möglichen Sensoren.
#define CUSTOM_RQ_TOGGLE 			3
#define CUSTOM_RQ_TOGGLE_LEN			0
