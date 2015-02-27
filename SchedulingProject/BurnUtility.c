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

void spinTest(){
  int test_num = 250;
  int target = 100;
  printf("Running %d burns:\n",test_num);
  int max = -1;
  int min = -1;
  long total = 0;
  int peaks = 0;

  struct timespec start;
  struct timespec end;

  int iter;
  for(iter = 0; iter < test_num; iter++){
    clock_gettime(CLOCK_REALTIME, &start);
    spin( target * 1000 );
    clock_gettime(CLOCK_REALTIME, &end);
    long diff = (timespec2nsec(&end) - timespec2nsec(&start))/1000000;\
    if (diff > max || max == -1){
      max = diff;
    }
    if (diff < min || min == -1){
      min = diff;
    }
    if (diff > target * 1.25 || diff < target * .75 ){
      peaks++;
    }
    total += diff;
  }
  printf("\tmax %d, min %d, avg %f, peaks %d\n", max, min, total/1.0/test_num, peaks);
}


