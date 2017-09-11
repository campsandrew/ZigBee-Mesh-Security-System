
#include "serial.h"

void initSerial(){
  UBRR0L = 103; // Baud rate 9600
  UCSR0B |= (1 << RXEN0 | 1 << TXEN0 | 1 << RXCIE0);// | 1 << RXCIE0); // Enable receiver and transmitter
  UCSR0C = (1 << USBS0) | (3 << UCSZ00); // Set frame format: 8 data, 2 stop bits
}

void transmit_data(unsigned char data){
  while(!(UCSR0A & (1 << UDRE0))); // Wait for empty transmit buffer

  UDR0 = data; // Put data into buffer, sends the data
}

unsigned char receive_data(){
  //while(!(UCSR0A & (1 << RXC0))); // Wait for data to be received
  return UDR0; // Get and return received data from buffer
}

//void flush_data(){
  //unsigned char dummy;
  //while(UCSR0A & (1 << RXC0)) dummy = UDR0;
//}
