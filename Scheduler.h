#ifndef SCHEDULER_H_
#define SCHEDULER_H_

typedef enum {
  RATE_MONOTONIC,
  EARLIEST_DEADLINE,
  LEAST_SLACK
} SCHED_ALG;

extern int scheduleTask( Workload *workload );

#endif /* Scheduler.h */