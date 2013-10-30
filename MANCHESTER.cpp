/*
This is a port of Manchester libraries to the Raspberry PI
Made by Sarrailh Rémi (maditnerd@gmail.com)
As for now it only works for sending messages


This code is based on the Atmel Corporation Manchester
Coding Basics Application Note.

http://www.atmel.com/dyn/resources/prod_documents/doc9164.pdf

Quotes from the application note:

"Manchester coding states that there will always be a transition of the message signal 
at the mid-point of the data bit frame. 
What occurs at the bit edges depends on the state of the previous bit frame and
does not always produce a transition. A logical “1” is defined as a mid-point transition
from low to high and a “0” is a mid-point transition from high to low.

We use Timing Based Manchester Decode.
In this approach we will capture the time between each transition coming from the demodulation
circuit."

Timer 2 is used with a ATMega328. Timer 1 is used for a ATtiny85.

This code gives a basic data rate as 1000 bits/s. In manchester encoding we send 1 0 for a data bit 0.
We send 0 1 for a data bit 1. This ensures an average over time of a fixed DC level in the TX/RX.
This is required by the ASK RF link system to ensure its correct operation.
The actual data rate is then 500 bits/s.
*/

#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include "MANCHESTER.h"

#define HALF_BIT_INTERVAL 1000 // microseconds

//Constructeur par défaut Paramètre le PIN du transmetteur sur 11
MANCHESTERClass::MANCHESTERClass()  
{
  wiringPiSetup();
  TxPin = TxDefault;
  pinMode(TxPin, OUTPUT);
}//end of constructor

void MANCHESTERClass::SetTxPin(char pin)
{
  TxPin = pin;      // user sets the digital pin as output
  pinMode(TxPin, OUTPUT);      // sets the digital pin 4 default as output   
}//end of set transmit pin	

void MANCHESTERClass::Transmit(unsigned int data)
{
  unsigned char byteData[2] = {data >> 8, data & 0xFF};
  TransmitBytes(2, byteData);
}

/*
The 433.92 Mhz receivers have AGC, if no signal is present the gain will be set 
to its highest level.

In this condition it will switch high to low at random intervals due to input noise.
A CRO connected to the data line looks like 433.92 is full of transmissions.

Any ASK transmission method must first sent a capture signal of 101010........
When the receiver has adjusted its AGC to the required level for the transmisssion
the actual data transmission can occur.

We send 14 0's 1010...    It takes 1 to 3 10's for the receiver to adjust to 
the transmit level.

The receiver waits until we have at least 10 10's and then a start pulse 01.
The receiver is then operating correctly and we have locked onto the transmission.
*/
void MANCHESTERClass::TransmitBytes(unsigned char numBytes, unsigned char *data)
{
  // Setup last send time so we start transmitting in 10us
  lastSend = micros() - HALF_BIT_INTERVAL + 10;

  // Send 14 0's
  for( int i = 0; i < 14; i++) //send capture pulses
    sendzero();  //end of capture pulses
 
  // Send a single 1
  sendone();  //start data pulse
 
  // Send the user data
  for (unsigned char i = 0; i < numBytes; i++)
  {
    unsigned int mask = 0x01; //mask to send bits 
    for (char j = 0; j < 8; j++)
    {
      if ((data[i] & mask) == 0)
        sendzero();
      else
        sendone();
      mask = mask << 1; //get next bit
    }//end of byte
  }//end of data

  // Send 2 terminatings 0's
  sendzero();
  sendzero();  
}//end of send the data	

void MANCHESTERClass::sendzero(void)
{
  delayMicroseconds(lastSend + HALF_BIT_INTERVAL - micros());        
  digitalWrite(TxPin, HIGH);   

  delayMicroseconds(HALF_BIT_INTERVAL);        
  digitalWrite(TxPin, LOW);
 
  lastSend = micros();
}//end of send a zero

void MANCHESTERClass::sendone(void)
{
  delayMicroseconds(lastSend + HALF_BIT_INTERVAL - micros());        
  digitalWrite(TxPin, LOW);   

  delayMicroseconds(HALF_BIT_INTERVAL);        
  digitalWrite(TxPin, HIGH);
 
  lastSend = micros();
}//end of send one

MANCHESTERClass MANCHESTER;