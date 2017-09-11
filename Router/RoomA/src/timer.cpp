// Description: This file implements functions that utilize the timers
//----------------------------------------------------------------------//

#include "timer.h"

void startTimer3(){
  TCCR3B |= (1 << CS30 | 1 << CS31);
  TCNT3 = 0;
}

void stopTimer3(){
  TCCR3B &= ~(1 << CS30 | 1 << CS31);
}

void initTimer0(){
  // set the timer mode to be "normal"
  TCCR0A &= ~(1 << WGM00 | 1 << WGM01);
  // set clock/prescalar for timer 0 to be clk/1024
  TCCR0B &= ~(1 << WGM02);
  //enable the interrupts
  TIMSK0 |= (1 << TOIE0);
}

void startDisplayTimer(){
  TCCR0B |= (1 << CS00 | 1 << CS02);
}

void stopDisplayTimer(){
  TCCR0B &= ~(1 << CS00 | 1 << CS01 | 1 << CS02);
}

/* Initialize timer 1, you should not turn the timer on here. You will need to
* use CTC mode
*/
void initTimer1(){

  //Values of TCCR1 for CTC Mode
  TCCR1A &= ~(1 << WGM11 | 1 << WGM10);
  TCCR1B |= (1 << WGM12);
  TCCR1B &= ~(1 << WGM13);

  // Turn off the timer
  TCCR1B &= ~(1 << CS10 | 1 << CS11 | 1 << CS12);
}

/* This delays the program an amount specified by unsigned int delay.
*/
void delayUs(unsigned int delay){

  unsigned int ticks = (16 * delay) - 1;

  // clear the timer
  TCNT1H = 0;
  TCNT1L = 0;

  // Turn on the timer with no prescaler
  TCCR1B &= ~(1 << CS11 | 1 << CS12);
  TCCR1B |= (1 << CS10);

  // calculate the TOP value and assign it to OCR1A
  OCR1AH = ticks >> 8;
  OCR1AL = ticks & 0x00FF;

  // Do nothing while the OCF1A flag is not up
  while(!(TIFR1 & (1 << OCF1A)));

  // Clear the OCF1A flag
  TIFR1 |= (1 << OCF1A);

  // Turn off the timer
  TCCR1B &= ~(1 << CS10 | 1 << CS11 | 1 << CS12);

}

void delayMs(unsigned int delay){

  unsigned int ticks = (250 * delay) - 1;

  // clear the timer
  TCNT1H = 0;
  TCNT1L = 0;

  // Turn on the timer with prescalar 64
  TCCR1B |= (1 << CS10 | 1 << CS11);
  TCCR1B &= ~(1 << CS12);

  // calculate the TOP value and assign it to OCR1A
  OCR1AH = ticks >> 8;
  OCR1AL = ticks & 0x00FF;

  // Do nothing while the OCF1A flag is not up
  while(!(TIFR1 & (1 << OCF1A)));

  // Clear the OCF1A flag
  TIFR1 |= (1 << OCF1A);

  // Turn off the timer
  TCCR1B &= ~(1 << CS10 | 1 << CS11 | 1 << CS12);

}
