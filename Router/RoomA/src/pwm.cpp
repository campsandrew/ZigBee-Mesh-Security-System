#include "pwm.h"
#include "timer.h"

// Clock is 16 MHz
void initPWM(){
  TCCR1A &= ~(1 << COM1A1);
  TCCR1A |= (1 << COM1A0);

  // Set the pin 11 on board as output
  DDRH |= (1 << DDH5);


}

void buzzerOn(){
  delayUs(200);
  PORTH |= (1 << PORTH5);
  delayUs(200);
  PORTH &= ~(1 << PORTH5);
}
