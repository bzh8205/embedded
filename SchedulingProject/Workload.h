/**
 * Holds structures required of many many functions
 */

#ifndef WORKLOAD_H_
#define WORKLOAD_H_

#include <time.h>
#include <sys/dispatch.h>

typedef enum {
  RATE_MONOTONIC,
  EARLIEST_DEADLINE,
  LEAST_SLACK
} SCHED_ALG;

typedef enum {
  //prompting scheduler to pick task,
  SCHED_START, // EVENT 0, info: sched_ctr
  //no task was scheduled
  NOTHING_SCHED, //EVENT 1, info: 0
  //task was scheduled
  TASK_SCHED, //EVENT 2, info: task id
  //task spin start,
  TASK_EXEC_START, //EVENT 3, info: task id
  //task spin end
  TASK_EXEC_END, //EVENT 4, info: task id
  //start of entire test
  START_TEST, //EVENT 5, info: 0
  //end of entire test
  END_TEST //EVENT 6, info: 0
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
  unsigned int ch_id;
  unsigned int thread_id;
  unsigned int exec_time_us;
} TaskArguments;

typedef struct {
  int task_num;
  Task ** tasks;
//TODO  TaskThread *threads;
} Workload;

typedef struct {
  long exec_time_us;
  int exec_number;
  int deadlines_missed;
} TaskStats;

typedef struct {
  int start_time_ms;
  int end_time_ms;
  long idle_time_us;
  long exec_time_us;
  int exec_num;
  int idle_num;
  int total_deadlines_missed;
  TaskStats ** task_stats;
} Stats;

#endif //Workload.h
