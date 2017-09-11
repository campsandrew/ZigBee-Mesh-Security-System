// Author:
// Net ID:
// Date:
// Assignment:
//
// Description:
//----------------------------------------------------------------------//
#include "sensor.h"
#include "timer.h"
#include <avr/io.h>

void initSensor(){
  //Trigger pin
  DDRK |= (1 << DDK6); //Intializes for output
  //Echo pin
  DDRK &= ~(1 << DDK7); //Intialize for input

  // enable pin-change interrupts
  PCICR |= (1 << PCIE2); //PCINT[7:0]
  // enable interrupts on PB3
  PCMSK2 |= (1 << PCINT23); //PCINT[3]
  //EICRA |= (1 << ISC20);  // set interrupt triggering logic change
  //EIMSK |= (1 << INT2); // enable external interrupt request 0 (was GICR)
}

void sensorOn(){
  PORTK |= (1 << PORTK6);  // trigger sensor
}

void sensorOff(){
  PORTK &= ~(1 << PORTK6);  // trigger sensor
}

void trigger(){
  delayMs(100);
  sensorOff();
  delayUs(4);
  sensorOn();
  delayUs(15);
  sensorOff();
  startTimer3();
}
