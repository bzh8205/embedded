#ifndef PROJECT1_H_
#define PROJECT1_H_

#include "Workload.h" //need for enum SCHED_ALG

#define NS_CLOCK_SPEED 10000 //10us 


/**
 * \brief inits workload structure given some set of tasks and size 
 */
int initWorkLoad(Workload* wl, unsigned int test[][3], int testSize );

/**
 * \brief inits stats structure
 */
int initStats(Workload* wl, Stats* stats);

/**
 * \brief updates the stats structure. If taskId is -1 it updates
 * the idles time stats.
 */
void updateStats( int taskId, Workload *wl, int startCycles,
    int endCycles, Stats* stats);

/**
 * \brief runs the actual test. Prompts the scheduler for tasks to run
 * and runs them.
 */
void _runTest(time_t startTime, Workload* wl, SCHED_ALG alg, Stats* stats);

/**
 * \brief timing duration wrapper, calls _runTest to utilize scheduler
 * and spin for tasks exec time
 */
void runTest(Workload* wl, SCHED_ALG alg, Stats* stats);

/**
 * \brief generic logging method. It will either output TRACE_EVENTS
 * or printf's but is easily switchable
 */
void logEvent( EVENT_TYPE et, int info );

/**
 * \brief destroys/frees the workload structure given the number of 
 * tasks initilized with
 */
int destroyWorkLoad(Workload* wl, int testSize );

/**
 * \brief destroys frees the stats structure given the number of tasks
 * it was initialized with
 */
int destroyStats(Stats* stats, int testSize);

#endif /* Project1.h */
