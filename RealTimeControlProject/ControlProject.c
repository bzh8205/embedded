#include <stdlib.h>
#include <stdio.h>

#include "ADC.h"
#include "ThreadRunner.h"

int main(int argc, char *argv[]) {
    short vin;
    //Init ADC thread on channel 1
    //TODO in thread creation
    if ( ! GetRootAccess() )
    {
        SetupAtoD() ;
        vin = MeasureVoltageOnChannel (1) ; //test read
        printf("Just read %6d V\n",vin); 
        //TODO make a channel read that doesn't write the channel at every call
    }
    else {
        printf( "\nFailure getting root access for I/O register mapping\n") ;
        return EXIT_SUCCESS;
    }

    //Init DAC on channel 0

	printf("Welcome to the QNX Momentics IDE\n");

	startThreads();

	return EXIT_SUCCESS;
}
