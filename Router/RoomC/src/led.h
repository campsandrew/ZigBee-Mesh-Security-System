// Description: This file contains function prototypes to be implemented in
// led.cpp and to be included in main.cpp.
//----------------------------------------------------------------------//

#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <avr/io.h>

#define LED PORTB7

void initLED();
void LED_On();
void LED_Off();

#endif
