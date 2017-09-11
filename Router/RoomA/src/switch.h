// Description: Function prototypes for switch.cpp
//----------------------------------------------------------------------//

#ifndef SWITCH_H
#define SWITCH_H

#include <avr/io.h>

#define ARM_BUTTON PORTB3
#define DISPLAY_BUTTON PORTJ1

void initArmButton();
void initDisplayButton();

#endif
