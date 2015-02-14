/**
 * Measures used for task execution
 */
#include <timer.h>
#include "BurnUtility.h"

/**
 * \brief burns cpu cycles for supposed time
 */
void spin( int us ){
	int i;
	unsigned int endSpin = (unsigned int) fudge*us*US_10_EXE;// 
	for (i = 0; i< endSpin; i++) {
		//TODO some math stuff, use for exe at 10us increments
	}
}

/**
 * Sets the fudge factor for spin funct
 */
void initSpinUtility(){
	int i;
	clock_t start,finish;
	double actualT;
	fudge = 1; //TODO might want to initialize at start
	//clock OR use getTime
	start = clock();
	//test for 10ms spin
	spin (10000); // 10ms / 10us = 1000
	//clock
	finish = clock();
	actualT =(double)((finish-start)*1000/CLOCKS_PER_SEC);
	fudge = actualT/10.0;  // fraction of recieved over expected
}


