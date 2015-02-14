#ifndef PROJECT1_H_
#define PROJECT1_H_

#define NS_CLOCK_SPEED 10000 //10us 

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
  Task *tasks;
//TODO  TaskThread *threads;
} Workload;

/**
 * \brief inits workload structure given some set of tasks and size 
 */
int initWorkLoad(Workload* wl, unsigned int ** test, int testSize );

#endif /* Project1.h */
