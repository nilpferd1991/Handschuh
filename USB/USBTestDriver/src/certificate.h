/* Name: certificate.c
 * Project: Handschuh Controller - die Firmware für den Handschuh
 * Author: Nils Braun
 */

/*
 * Hier werden alle möglichen RequestCodes für die Kommunikation zwischen Host und Handschuh definiert
 */

#ifndef CERTIFICATE_H_
#define CERTIFICATE_H_

// Buffer-Größe eines Buffers in unsigned char abgespeichert
// Größe ist in Byte angegeben
#define BUFFER_SIZE 32

// Das Echo-Kommande sendet eine 2-Byte-Nachricht zurück, die aus dem Value (2 Bytes) besteht.
// Es dient zum Test der Verbindung.
#define CUSTOM_RQ_ECHO 				0
#define CUSTOM_RQ_ECHO_LEN 			4

// Das DOIT-Kommande sendet eine 1-Byte-Nachricht zurück, die aus dem momentanen Counter (danach!) besteht.
// Es dient zum Ausführen des Kommandos.
#define CUSTOM_RQ_DOIT 				2
#define CUSTOM_RQ_DOIT_LEN 			1

// Das Data-Kommando fragt die momentanen Daten vom Handschuh ab. Die Antwort ist ein BUFFER_SIZE-Byte-Puffer
// mit den momentanen Sensordaten
#define CUSTOM_RQ_DATA 				1
#define CUSTOM_RQ_DATA_LEN			BUFFER_SIZE

// Die Offsets in die verschiedenen Richtungen geben an, wie stark die Normalweinstellung von 0,0,0 verschoben ist.
#define OFFSET_X 40
#define OFFSET_Y 0
#define OFFSET_Z -240

#define STEADY_STATE_THRESHOLD 1000
#define STEADY_STATE_CYCLES 10
#define MEASURE_CYCLES 2

#endif /* CERTIFICATE_H_ */
