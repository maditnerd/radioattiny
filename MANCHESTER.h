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

#ifndef MANCHESTER_h
#define MANCHESTER_h

#include <wiringPi.h>
#include <stdint.h>
//#define NULL 0
#define CHANGE 1

typedef uint8_t boolean;
typedef uint8_t byte;

#define TxDefault 11  //the digital pin to use to transmit data
#define pulse 1000 //the individual transmit pulse width in msec

#define MinCount 40  //pulse lower count limit on capture
#define MaxCount 85  //pulse higher count limit on capture
#define MinLongCount 103  //pulse lower count on double pulse
#define MaxLongCount 147  //pulse higher count on double pulse

#define TimeOutDefault -1  //the timeout in msec default blocks

class MANCHESTERClass
{
  public:
    MANCHESTERClass();  //the constructor
    void SetTxPin(char pin); //set the arduino digital pin for transmit. default 4.
    void Transmit(unsigned int data);  //transmit 16 bits of data
    void TransmitBytes(unsigned char numBytes, unsigned char *data); // transmit a byte array
    
  private:
    void sendzero(void);
    void sendone(void);
    unsigned char  TxPin;
    unsigned long lastSend;
};//end of class MANCHESTER

extern MANCHESTERClass MANCHESTER;

#endif
