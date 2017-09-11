// Description: This file describes functions used by the timer
//----------------------------------------------------------------------//

#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>

void initTimer1();
void startTimer3();
void stopTimer3();
void initTimer0();
void startDisplayTimer();
void stopDisplayTimer();
void delayUs(unsigned int delay);
void delayMs(unsigned int delay);

#endif
