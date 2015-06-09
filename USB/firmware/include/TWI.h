#pragma once
// I2C-Adresse des Beschleunigungssensors
// für Schreibe- und Lesevorgänge
#define SLA_W 0x70
#define SLA_R 0x71

void twiPoll();
void twiInit();
void twiToggle();
void twiGetData();
void twiSendWriteAddress();
void twiSendReadAddress();
