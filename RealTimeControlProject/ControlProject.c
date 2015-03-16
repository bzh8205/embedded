#include <stdlib.h>
#include <stdio.h>
<<<<<<< HEAD
#include "ADC.h"
=======
#include "ThreadRunner.h"
>>>>>>> 3eeb7d99f5f451aa85b0531252884cd5ca465099

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
        return EXIT_SUCESS;
    }

    //Init DAC on channel 0

	printf("Welcome to the QNX Momentics IDE\n");

	startThreads();
	endThreads();

	return EXIT_SUCCESS;
}
