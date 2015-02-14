#ifndef SCHEDULER_H_
#define SCHEDULER_H_

typedef enum {
  RATE_MONOTONIC,
  EARLIEST_DEADLINE,
  LEAST_SLACK
} SCHED_ALG;

unsigned int scheduleTask( Workload *workload, SCHED_ALG alg);

#endif /* Scheduler.h */
