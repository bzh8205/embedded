#ifndef PROJECT1_H_
#define PROJECT1_H_

#include "Workload.h" //need for enum SCHED_ALG

#define NS_CLOCK_SPEED 10000 //10us 


/**
 * \brief inits workload structure given some set of tasks and size 
 */
int initWorkLoad(Workload* wl, unsigned int test[][3], int testSize );

void runTest(Workload* wl,SCHED_ALG alg) ;

/**
 * \brief destroys/frees the workload structure given the number of 
 * tasks initilized with
 */
int destroyWorkLoad(Workload* wl,int testSize );

#endif /* Project1.h */
