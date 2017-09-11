// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 */
void initArmButton(){
  PORTB |= (1 << ARM_BUTTON);
  DDRB &= ~(1 << DDB3);
  // enable pin-change interrupts
  PCICR |= (1 << PCIE0); //PCINT[7:0]
  // enable interrupts on PB3
  PCMSK0 |= (1 << PCINT3); //PCINT[3]
}

void initDisplayButton(){
  DDRJ &= ~(1 << DDJ1); // Sets pullup resistor to input mode
  PORTJ |= (1 << DISPLAY_BUTTON);
  // enable pin-change interrupts
  PCICR |= (1 << PCIE1);
  // enable interrupts on PJ0
  PCMSK1 |= (1 << PCINT10); //PCINT[3]
}
