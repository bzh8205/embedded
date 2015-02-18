/**
 * Holds structures required of many many functions
 */

#ifndef WORKLOAD_H_
#define WORKLOAD_H_

#include <time.h>

typedef enum {
  RATE_MONOTONIC,
  EARLIEST_DEADLINE,
  LEAST_SLACK
} SCHED_ALG;

typedef enum {
  SCHED_START, //prompting scheduler to pick task
  NOTHING_SCHED, //no task was scheduled
  TASK_SCHED, //task was scheduled
  TASK_EXEC_START, //task spin start
  TASK_EXEC_END, //task spin end
  START_TEST, //start of entire test
  END_TEST //end of entire test
} EVENT_TYPE;

typedef struct {
  unsigned int id;
  unsigned int exec_time_us;
  unsigned int period_time_us;
  unsigned int deadline_us;
  unsigned int last_finish_us;
  unsigned int next_deadline_us;
  unsigned int last_exec_us;
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


typedef struct {
  time_t exec_cycles;
  int exec_number;
  int deadlines_missed;
} TaskStats;

typedef struct {
  time_t start_cycles;
  time_t end_cycles;
  time_t idle_cycles;
  time_t exec_cycles;
  int total_deadlines_missed;
  TaskStats ** task_stats;
} Stats;




#endif //Workload.h
