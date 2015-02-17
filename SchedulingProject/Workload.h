/**
 * Holds structures required of many many functions
 */

#ifndef WORKLOAD_H_
#define WORKLOAD_H_


typedef enum {
  RATE_MONOTONIC,
  EARLIEST_DEADLINE,
  LEAST_SLACK
} SCHED_ALG;

typedef struct {
  unsigned int id;
  unsigned int exec_time_us;
  unsigned int period_time_us;
  unsigned int deadline_us;
  unsigned int last_finish_us;
  unsigned int next_deadline_us;
} Task;

typedef struct {
  unsigned int thread_id;
  unsigned int exec_time_us;
} TaskArguments;

typedef struct {
  int task_num;
  Task ** tasks;
//TODO  TaskThread *threads;
} Workload;


#endif //Workload.h
