/**
 * Measures used for task execution
 */
#include <stdio.h>
#include <time.h>
#include "BurnUtility.h"

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
	clock_t start,finish;
	double actualT;
	//clock OR use getTime
	start = clock();
	//test for 10ms spin
	spin (10000); // 10ms / 10us = 1000
	//clock
	finish = clock();
	actualT =(double)((finish-start)*1000/CLOCKS_PER_SEC);
	fudge = 10.0/actualT;  // fraction of recieved over expected
	printf("Got:%f\nWant:10\nFudge:%f\n",actualT,fudge);
}


