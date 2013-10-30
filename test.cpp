//to compile it
//gcc test.cpp MANCHESTER.cpp -o test -lwiringPi
//Sarrailh Remi
#include "MANCHESTER.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char buffer[256];

int main(int argc, char *argv[]) {

strcpy(buffer,argv[1]); //Argument 1 is Tx Pin
unsigned int txPin = atoi(buffer);

  if (wiringPiSetup () == -1) return 1;

    MANCHESTER.SetTxPin(txPin);
    for (int i = 0; i < 15; ++i)
    {
	printf("SEND: 1234 ON GPIO %i\n",txPin);
    	MANCHESTER.Transmit(1234);
    }
}
