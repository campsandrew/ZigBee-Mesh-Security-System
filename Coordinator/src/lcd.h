// Description: This file implements turnOnLED and initLED to control
// the LEDs.
//----------------------------------------------------------------------//

#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <avr/io.h>
#include "timer.h"

void initLCD();
void initLCDPins();
void cursorDown();
void initLCDProcedure();
void enableLow();
void enableHigh();
void clearDisplay();
void displayOn();
void displayOff();
void rsLow();
void rsHigh();
void sendCommand();
void writeString(const char *string);
void writeCharacter(unsigned char character);
void writeDataWithUsDelay(unsigned char data, unsigned int delay);
void writeCommandWithUsDelay(unsigned char command, unsigned int delay);

#endif
