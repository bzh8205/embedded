#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "Workload.h" //need for workload


unsigned int scheduleTask( Workload *workload, SCHED_ALG alg, clock_t curTime);

#endif /* Scheduler.h */
