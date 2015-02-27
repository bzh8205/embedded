/**
 * Measures used for task execution
 */
#include <stdio.h>
#include <time.h>
#include "BurnUtility.h"
#include "Project1.h"	//adding new

/**
 * \brief burns cpu cycles for supposed time
 */
void spin( int us ){
	int i,sum;
	double endSpin = fudge*us*US_10_EXE;// 
	for (i = 0; i< endSpin; i++) {
		//TODO some math stuff, use for exe at 10us increments
		sum += sum;
	}
}

/**
 * Sets the fudge factor for spin funct
 */
void initSpinUtility(){
	int i;
	double actualT;
	//clock OR use getTime
	struct timespec start;
	struct timespec end;
	clock_gettime(CLOCK_REALTIME, &start);
	//test for 10ms spin
	spin (100000); // 100ms / 10us = 10000
	//clock
	clock_gettime(CLOCK_REALTIME, &end);
	//actualT =(double)((finish)*1000/CLOCKS_PER_SEC);
	//fudge = 1000.0/actualT;  // fraction of recieved over expected
	long runtime = (timespec2nsec(&end) - timespec2nsec(&start))/1000000;
	fudge = 100.0/runtime;
	printf("Got:%lu\nWant:100\nFudge:%f\n",runtime,fudge);
}


