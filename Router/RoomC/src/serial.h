#ifndef SERIAL_H
#define SERIAL_H

#include <Arduino.h>
#include <avr/io.h>

#define CPU_FREQ 16000000

void initSerial();
void transmit_data(unsigned char data);
unsigned char receive_data();
void flush_data();

#endif
