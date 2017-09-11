// Author:         Derek Paris, Andrew Camps, Kathleen Spencer, Jason Tran
// Net ID:         derekparis, andrewcamps, ichikasuto, krspencer
// Date:           30 March 2017
// Assignment:     Final Project
//
// Description:
//
// Requirements:
//----------------------------------------------------------------------//

#include <Arduino.h>
#include <avr/io.h>
#include "led.h"
#include "switch.h"
#include "timer.h"
#include "lcd.h"
#include "serial.h"
#include "sensor.h"
#include "pwm.h"

#define TRUE 1
#define FALSE 0
#define CM_CONVERT 58
#define CLK_PRESCALED 250000.0
#define TO_uS 1000000
#define DISPLAY_TIME_SEC 15

#define THIS_ROOM "Room B"

typedef enum StateType_enum{
  WAIT_ACTION, SENSING, ARM, ARM_RELEASE, ARM_PRESS_db, ARM_RELEASE_db,
  DISPLAY_RELEASE, DISPLAY_PRESS_db, DISPLAY_RELEASE_db, ALARM, INIT
}StateType;

volatile unsigned char readData;
volatile StateType state = WAIT_ACTION;
volatile StateType prevState;
volatile char received;

volatile int echo = FALSE;
volatile unsigned int uS = 0;
volatile unsigned int pulseTime = 0;
volatile unsigned int cm = 0;

volatile unsigned int roomDist = 0;
volatile int ready = FALSE;
volatile int display = TRUE;

volatile unsigned int tick_times = 0;

char roomA = '1';
char roomB = '2';
char roomC = '3';

char roomTriggered;

/*
* Method to call all initialization functions
*/
void initialize(){
  sei();
  initSerial();
  initTimer1();
  initTimer0();
  initLED();
  initLCD();
  initArmButton();
  initDisplayButton();
  initPWM();
  initSensor();
  displayOn();
  writeString("PRESS TO");
  cursorDown();
  writeString("INITIALIZE");
}

void resetDisplayTimer(){
  displayOn();
  startDisplayTimer();
  display = TRUE;
  tick_times = 0;
}

int main(){

  int armed = FALSE;
  int i = 0;
  int alarming = 0;

  initialize();

  while(TRUE){

    switch(state){
      case WAIT_ACTION:
        if (alarming) {
          buzzerOn();
        }
        break;
      case INIT:
        transmit_data('I');
        clearDisplay();
        writeString("DETECTING ROOM");
        for(i = 0; i < 25; i++){
          trigger();
        }
        clearDisplay();
        writeString("ROOM DETECTED");
        cursorDown();
        ready = TRUE;
        clearDisplay();
        writeString("DISARMED");
        cursorDown();
        writeString(THIS_ROOM);
        startDisplayTimer();
        state = WAIT_ACTION;
        break;
      case ARM:
        if(armed == 0){
          resetDisplayTimer();
          clearDisplay();
          writeString("ARMED");
          cursorDown();
          writeString(THIS_ROOM);
          armed = TRUE;
          state = SENSING;
        }else{
          resetDisplayTimer();
          clearDisplay();
          writeString("DISARMED");
          cursorDown();
          writeString(THIS_ROOM);
          LED_Off();
          armed = FALSE;
          alarming = 0;
          state = WAIT_ACTION;
        }
        break;
      case ARM_PRESS_db:
        delayMs(10);
        state = ARM_RELEASE;
        break;
      case ARM_RELEASE:
        break;
      case ARM_RELEASE_db:
        delayMs(10);
        if(ready){
          if(armed == TRUE){
            transmit_data('D');
          }else{
            transmit_data('A');
          }
          state = ARM;
        }else{
          state = INIT;
        }
        break;
      case DISPLAY_PRESS_db:
        delayMs(10);
        state = DISPLAY_RELEASE;
        break;
      case DISPLAY_RELEASE:
        break;
      case DISPLAY_RELEASE_db:
        delayMs(10);
        if(display == FALSE){
          resetDisplayTimer();
        }

        if(prevState == SENSING){
          state = SENSING;
        }
        state = WAIT_ACTION;
        break;
      case ALARM:
        resetDisplayTimer();
        stopDisplayTimer();
        clearDisplay();
        writeString("SYSTEM TRIGGERED");
        cursorDown();
        if(roomTriggered == roomA){
          writeString("BY: Room A");
        }else if(roomTriggered == roomB){
          writeString("BY: Room B");
        }else if(roomTriggered == roomC){
          writeString("BY: Room C");
        }
        LED_On();
        alarming = 1;
        state = WAIT_ACTION;
        break;
      case SENSING:
        trigger(); //Trigger the sensor
        if(cm < roomDist){
          for(int i = 0; i < 2; i++){
              transmit_data(roomB);
          }
          roomTriggered = roomB;
          state = ALARM;
        }
        break;

    }
  }

  return 0;
}

ISR(USART0_RX_vect){
  received = receive_data();

  if(state == WAIT_ACTION && received == 'I'){
    transmit_data('I'); //Coordinator only
    state = INIT;
  }
  
  if(state == WAIT_ACTION && received == 'A'){
    transmit_data('A'); //Coordinator only
    state = ARM;
  }

  if(state == SENSING && received == 'D'){
    transmit_data('D'); //Coordinator only
    state = ARM;
  }

  if(state == WAIT_ACTION && received == 'D'){
    transmit_data('D'); //Coordinator only
    state = ARM;
  }

  //Displaying Triggered Room
  if(state == SENSING && received == roomA){
    roomTriggered = roomA;
    //Coordinator only
    for(int i = 0; i < 2; i++){
        transmit_data(roomA);
    }
    state = ALARM;
  }

  if(state == SENSING && received == roomC){
    roomTriggered = roomC;
    //Coordinator only
    for(int i = 0; i < 2; i++){
        transmit_data(roomC);
    }
    state = ALARM;
  }
}

/* ISR for sensor */
ISR(PCINT2_vect){

  if(echo == TRUE){ //Should be opposite change later
    pulseTime = TCNT3;
    stopTimer3();
    uS = (pulseTime / CLK_PRESCALED) * TO_uS;
    cm = uS / CM_CONVERT - 10;// - 10;

    if(!ready){
      if(roomDist == 0 || cm < roomDist){
        roomDist = cm;
      }
    }
    echo = FALSE;
  }else{
    echo = TRUE;
  }

}

ISR(TIMER0_OVF_vect){
  TIFR0 |= (1 << TOV0); // clears the timer overflow flag
  tick_times++;
  if(tick_times == (DISPLAY_TIME_SEC * 61)){
    displayOff();
    display = FALSE;
  }
}

ISR(PCINT0_vect){
  switch(state){
    case WAIT_ACTION:
    case SENSING:
      state = ARM_PRESS_db;
      break;
    case INIT:
      break;
    case ARM_RELEASE:
      state = ARM_RELEASE_db;
      break;
    case DISPLAY_RELEASE:
      break;
    case ALARM:
      break;
    case ARM_PRESS_db:
      break;
    case ARM_RELEASE_db:
      break;
    case DISPLAY_PRESS_db:
      break;
    case DISPLAY_RELEASE_db:
      break;
    case ARM:
      break;
  }
}

/* Display and Arm ISR */
ISR(PCINT1_vect){
  switch(state){
    case WAIT_ACTION:
      prevState = WAIT_ACTION;
      state = DISPLAY_PRESS_db;
      break;
    case INIT:
      break;
    case DISPLAY_RELEASE:
      state = DISPLAY_RELEASE_db;
      break;
    case ALARM:
      break;
    case ARM_RELEASE:
      break;
    case SENSING:
      prevState = SENSING;
      state = DISPLAY_PRESS_db;
      break;
    case ARM_PRESS_db:
      break;
    case ARM_RELEASE_db:
      break;
    case DISPLAY_PRESS_db:
      break;
    case DISPLAY_RELEASE_db:
      break;
    case ARM:
      break;
  }
}
