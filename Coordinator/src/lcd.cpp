// Description: This file implements functions to operate an LCD
//----------------------------------------------------------------------//

#include "lcd.h"

/* Asserts Logic High on Enable Pin
*/
void enableHigh(){
  PORTB |= (1 << PORTB4);
}

/* Asserts Logic Low on Enable Pin
*/
void enableLow(){
  PORTB &= ~(1 << PORTB4);
}

/* Asserts Logic Low on RS Pin
*/
void rsLow(){
  PORTH &= ~(1 << PORTH6);
}

/* Asserts Logic High on RS Pin
*/
void rsHigh(){
  PORTH |= (1 << PORTH6);
}

/* Initializes Tri-state for LCD pins and calls initialization procedure.
* This function is made so that it's possible to test initLCDPins separately
* from initLCDProcedure which will likely be necessary.
*/
void initLCD(){
  initLCDPins();
  initLCDProcedure();
}

/* Asserts enableHigh, delays, asserts enableLow, then delays again according to
* the specification on the data sheet. This function is meant to be called after
* the data pins have been set at appopriate values. This effectively "sends" the
* bits that are set on the data pins.
*/
void sendCommand(){
  enableHigh();
  delayUs(1);
  enableLow();
  delayUs(1);
}

/* Initializes all pins related to the LCD to be outputs
*/
void initLCDPins(){
  DDRA |= (1 << DDA0) | (1 << DDA1) | (1 << DDA2) | (1 << DDA3); //Initialize all pins to outputs
  DDRB |= (1 << DDB4); //Enable and RS pin
  DDRH |= (1 << DDH6);
  DDRB |= (1 << DDB6); //Backlight pin
}

/* 1. Assert bits to pins connected to DB[7:4] on the LCD screen
*      a. data is an unsigned char which has 8 bits. Therefore, you
*         need assign the bottom 4 bits of "data" to the appropriate bits in
*         PORTA
*  2. This is a "data" signal, meaning RS should be Low
*  3. send the command!
*  4. delay the provided number in MICROseconds.
*/
void writeDataWithUsDelay(unsigned char data, unsigned int delay){
  //PORTA = (PORTA & 0xF0) | (num & 0x0F);
  //PORTA &= 0x00;
  PORTA = (PORTA & 0xF0) | (data & 0x0F);
  rsLow();
  sendCommand();
  delayUs(delay);

}

/* Turns the LCD backlight on */
void displayOn(){
  PORTB |= (1 << PORTB6);
  writeDataWithUsDelay(0x00, 0);
  writeDataWithUsDelay(0x0C, 50);
  delayMs(25);
}

/* Turns the LCD backlight off */
void displayOff(){
  PORTB &= ~(1 << PORTB6);
  writeDataWithUsDelay(0x00, 0);
  writeDataWithUsDelay(0x08, 50);
  delayMs(25);
}

void cursorDown(){
  writeDataWithUsDelay(0x0C, 0);
  writeDataWithUsDelay(0x00, 50);
}

void clearDisplay(){
  writeDataWithUsDelay(0x00, 0);
  writeDataWithUsDelay(0x01, 50);
  delayMs(50);
}

/* Similar to writeDataWithUsDelay except that now all eight bits of command are
* sent.
* 1. Assert bits to pins connected to DB[7:4] on the LCD screen
*       a. unlike writeDataWithUsDelay, you need to send the TOP four bits of
*          "command" first. These should be assigned to appropriate bits in PORTA
* 2. This is a command signal, meaning RS should be High
* 3. send the command!
* 4. Now set the lower four bits of command to appropriate bits in PORTA
* 5. Send the command!
* 6. delay the provided number in MICROseconds.
*/
void writeCommandWithUsDelay(unsigned char command, unsigned int delay){

  PORTA = (PORTA & 0xF0) | ((command & 0xF0) >> 4); //Set High bits
  rsHigh();
  sendCommand();
  PORTA = (PORTA & 0xF0) | (command & 0x0F); //Set Low bits
  sendCommand();
  delayUs(delay);

}

/* Writes a character to the LCD screen. This is done by giving a character
* such as 'G' to writeCommandWithUsDelay and delaying according to the datasheet.
*/
void writeCharacter(unsigned char character){
  writeCommandWithUsDelay(character, 50);
}

/* Writes a provided string such as "Hello!" to the LCD screen. You should
* remember that a c string always ends with the '\0' character
*/
void writeString(const char *string){
  int i = 0;

  for(i = 0; string[i] != '\0'; i++){
    writeCharacter(string[i]);
  }
}

/* This is the procedure outline on the LCD datasheet page 4 out of 9.
* This should be the last function you write as it largely depends on all other
* functions working.
*/
void initLCDProcedure(){

  // Delay 15 milliseconds
  delayMs(15);

  // Write 0b0011 to DB[7:4] and delay 4100 microseconds
  writeDataWithUsDelay(0x3, 4100);

  // Write 0b0011 to DB[7:4] and delay 100 microseconds
  writeDataWithUsDelay(0x3, 100);

  // The data sheet does not make this clear, but at this point you are issuing
  // commands in two sets of four bits. You must delay after each command
  // (which is the second set of four bits) an amount specified on page 3 of
  // the data sheet.
  // write 0b0011 to DB[7:4] and no delay
  writeDataWithUsDelay(0x3, 0);

  // write 0b0010 to DB[7:4] and delay.
  writeDataWithUsDelay(0x2, 50);

  // Function set in the command table
  writeDataWithUsDelay(0x02, 0);
  writeDataWithUsDelay(0x08, 50);

  // Display off in the command table
  writeDataWithUsDelay(0x00, 0);
  writeDataWithUsDelay(0x08, 50);

  // Clear display in the command table. Remember the delay is longer!!!
  writeDataWithUsDelay(0x00, 0);
  writeDataWithUsDelay(0x01, 2000);

  // Entry Mode Set in the command table.
  writeDataWithUsDelay(0x00, 00);
  writeDataWithUsDelay(0x06, 50);

  // Display ON/OFF Control in the command table. (Yes, this is not specified),
  // in the data sheet, but you have to do it to get this to work. Yay datasheets!)
  writeDataWithUsDelay(0x00, 00);
  writeDataWithUsDelay(0x0C, 50);

}
