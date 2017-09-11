// Description: This file implements turnOnLED and initLED to control
// the LEDs.
//----------------------------------------------------------------------//


#include "led.h"

/*
 * Initializes pins B6, B5, and B4 to be in output mode using the DDR register
 * and bit operations. Other bits in DDRB must not be affected by this
 * assignment.
 */
void initLED(){
  DDRB |= (1 << DDB7); //Intializes the ports to output for LEDs
}

/*
* Turns on LED
*/
void LED_On(){
  PORTB |= (1 << LED); //Turns on Red LED
}

/*
* Turns off LED
*/
void LED_Off(){
  PORTB &= ~(1 << LED);
}
