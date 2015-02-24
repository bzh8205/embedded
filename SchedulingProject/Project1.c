//TODO resolve imports
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "BurnUtility.h"
#include "UserTracing.h"
#include "Scheduler.h"
#include "Project1.h"

#define FACTOR 100	//factor used for easy changing of task workload weights

//example workload descriptor
#ifdef TEST3
unsigned int test1[4][3] = { {1,4,4}, {2,5,5}, {1,8,8}, {1,10,10} }; 
int test1Size = 4;
#elif TEST2
//lazy test switching
unsigned int test1[3][3]= { {1,3,3}, {2,5,5}, {1,10,10} };
int test1Size = 3;
#else 
//example workload descriptor
unsigned int test1[5][3] = { { 1, 7, 7 }, { 2, 5, 5 }, { 1, 8, 8 },
    { 1, 10, 10 }, { 2, 16, 16 } };
int test1Size = 5;
#endif


int main(int argc, char *argv[]) {
  Workload wl;
  Stats stats;
  int i;
  fudge = 1; //defaulting fudge variable before test adjustment
  //TODOinitBurnUtility();
  initSpinUtility();
#ifndef ALYSSA_TESTING
  initUserTracing( argv[0] );
#endif
  //printf("Fudge: %f\n",fudge);
  //create workload struct from descriptor

  if (!initWorkLoad(&wl, test1, test1Size)) {
    printf("Error initializing Workload struct\n");
  }

  if (!initStats(&wl, &stats)) {
    printf("Error initializing Stats struct\n");
  }

  //check
  for (i = 0; i < test1Size; i++) {
    printf("id:%d C:%d P:%d D:%d LF:%d ND:%d LE:%d\n", (wl.tasks[i])->id,
        (wl.tasks[i])->exec_time_us, (wl.tasks[i])->period_time_us,
        (wl.tasks[i])->deadline_us, (wl.tasks[i])->last_finish_us,
        (wl.tasks[i])->next_deadline_us, (wl.tasks[i])->last_exec_us);
  }

  //runTest(&wl, EARLIEST_DEADLINE, &stats);
  runTest(&wl, LEAST_SLACK, &stats);
#ifdef ALYSSA_TESTING
  //print out the stats
  displayStats(&stats,test1Size);
#endif
  //free stats
  destroyStats(&stats, test1Size);

  //free workload
  destroyWorkLoad(&wl, test1Size);
  return 0;
}

/**
 * \brief Initializes the workload structure provided with the task list
 * provided 
 * \param wl the workload structure to initialize
 * \param test a list of tasks, with there C,P, and D
 * \param testSize number of tasks in test list 
 * \return returns whether init was successful
 */
int initWorkLoad(Workload* wl, unsigned int test[][3], int testSize) {
  int i;
  Task** tptr = malloc(sizeof(Task*) * testSize);//create task ptr array
  for (i = 0; i < testSize; i++) {
    tptr[i] = malloc(sizeof(Task)); // create task structure
    //printf("created struct %d in task array\n",i);
    tptr[i]->id = i;
    //TODO remove FACTOR factors
    tptr[i]->exec_time_us = test[i][0] *FACTOR;
    tptr[i]->period_time_us = test[i][1] *FACTOR;
    tptr[i]->deadline_us = test[i][2] *FACTOR;
    tptr[i]->last_finish_us = 0; //default
    tptr[i]->next_deadline_us = test[i][2]*FACTOR; //default TODO remove x10
    tptr[i]->last_exec_us = 0; //default
    //printf("done w/ struct %d\n", i);
  }
  //add to struct
  wl->task_num = testSize;
  wl->tasks = tptr;

  return 1;
}

int initStats(Workload* wl, Stats* stats) {
  int i;
  int task_num = wl->task_num;
  stats->start_cycles = 0;//clock; //TODO
  stats->end_cycles = 0;
  stats->idle_cycles = 0;
  stats->exec_cycles = 0;
  stats->total_deadlines_missed=0;//; //TODO

  TaskStats ** ts_ptr = malloc(sizeof(TaskStats*) * task_num);
  for (i = 0; i < task_num; i++) {
    ts_ptr[i] = malloc(sizeof(TaskStats));
    ts_ptr[i]->exec_cycles = 0;
    ts_ptr[i]->exec_number = 0;
    ts_ptr[i]->deadlines_missed = 0;
  }
  stats->task_stats = ts_ptr;

  return 1;
}

//Updates idle time if taskId = -1, updates a task stats otherwise.
void updateStats(int taskId, Workload* wl, int startCycles, int endCycles,
    Stats* stats) {
  int i;//iterator
  unsigned int l_dl; //last deadline calculation
  if (taskId == -1 && (startCycles > stats->start_cycles)) {
    stats->idle_cycles = (endCycles - startCycles);
  } else if (taskId != -1){ //checking for non-neg index, led to segfault
    stats->exec_cycles = endCycles - startCycles;
    (stats->task_stats[taskId])->exec_cycles += (endCycles - startCycles);
    (stats->task_stats[taskId])->exec_number += 1;
    
    //TODO: check if deadline was missed.
  } else {
    //TODO?
  } 
/* 
  for (i =0;i< wl->task_num;i++) {
    l_dl = (wl->tasks[i])->next_deadline_us- (wl->tasks[i])->deadline_us;
    if (  ( (wl->tasks[i])->last_exec_us < l_dl) &&
	(endCycles > (wl->tasks[i])->next_deadline_us) ) { 
       //didn't execut within last period and passed its deadline
	//TODO may update for same deadline multiple times, might require a ran flag
      (stats->task_stats[i])->deadlines_missed +=1;
      stats->total_deadlines_missed +=1;
    }
  }
  */
}

void logEvent( EVENT_TYPE et, int info ){
  //TODO
}

void printTaskInfo(Task* t) {
         printf("[%d] C:%d P:%d D:%d LE:%d LF:%d ND:%d\n", t->id,
        t->exec_time_us , t->period_time_us ,
        t->deadline_us , t->last_exec_us,
        t->last_finish_us, t->next_deadline_us );
}

/**
 * \brief Updates the next deadline if deadlines were missed and not automatically
 * updated after execution
 */
void updateDeadlines(clock_t lastClock, Workload* wl,Stats * stats) {
   int id;
   for (id=0 ; id< wl->task_num; id++) {
	//TODO, error with comparing this inequality
      if ( lastClock > (unsigned long) (wl->tasks[id])->next_deadline_us ) { //if DL passed 
//printf("ceil: %f of %lu/%u=%f\n", ceil( (double) lastClock/(wl->tasks[id])->deadline_us), lastClock, (wl->tasks[id])->deadline_us
//		,(double) lastClock/(wl->tasks[id])->deadline_us);
         (wl->tasks[id])->next_deadline_us = (unsigned int)
		ceil ( (double)lastClock/(wl->tasks[id])->deadline_us) * (wl->tasks[id])->deadline_us;

		//update the deadline missed
//printf("UD[%d]:%d\n",id, (wl->tasks[id])->next_deadline_us);
		(stats->task_stats[id])->deadlines_missed +=1;
		stats->total_deadlines_missed +=1;
      } 
   }
   return;
}

void _runTest(time_t startTime, Workload* wl, SCHED_ALG alg, Stats* stats){
  int id;
  clock_t pre_exec = 0;
  clock_t post_exec = 0;
  int sched_ctr = 0;
  clock_t tick=0;
  //while time < configuration.test_duration
  tick = clock();
  while (tick< startTime + 200*FACTOR) {//TODO remove FACTOR
    updateDeadlines(tick-startTime, wl,stats); 
   //TODO not updating fast enough, times between execution too costly
    sched_ctr++;
    logEvent( SCHED_START, sched_ctr );
    id = scheduleTask(wl, alg);
    if (id != -1) {
      logEvent( TASK_SCHED, id );
      //LOG: start task spin
      pre_exec = clock(); //TODO by this time first deadline has passed!
//#ifndef ALYSSA_TESTING
      updateStats(-1, wl, post_exec, pre_exec, stats);
//#endif
      //printf("starting task %d at: %lu\n",id,pre_exe);
      logEvent( TASK_EXEC_START , id );
      spin((wl->tasks[id])->exec_time_us);
      logEvent( TASK_EXEC_END , id );
      //LOG: end task spin
      post_exec = clock();
//#ifdef ALYSSA_TESTING
      updateStats(id, wl, pre_exec, post_exec, stats);
//#endif
      //update workload
      (wl->tasks[id])->last_finish_us = post_exec-startTime;
      (wl->tasks[id])->last_exec_us = pre_exec-startTime;
      (wl->tasks[id])->next_deadline_us +=(wl->tasks[id])->deadline_us;
#ifdef ALYSSA_TESTING
     // printf("[%d]\n",id);
        printf("%lu\n",tick-startTime);
        printTaskInfo( (wl->tasks[id]) );
#endif
      //bad for wiggle spins
      //printf("took %lu\n", post_exe - pre_exe);
    } else {
#ifdef ALYSSA_TESTING
      printf("Nothing Scheduled\n");
      //updateDeadlines(clock(), wl);
printf("%lu\n",tick-startTime);
#endif
      logEvent( NOTHING_SCHED, 0 );
    }
    tick = clock(); //next tick
  } //done test
}

//TODO: params and returns
void runTest(Workload* wl, SCHED_ALG alg, Stats* stats) {
  clock_t startTime, endTime;
  //save start time
  startTime = clock();
  logEvent( START_TEST, 0 );
  stats->start_cycles = startTime;
  printf("~~~~~~~~~~Starting Sim at %lu~~~~~~~~~~~~\n\n", startTime);
  _runTest( startTime, wl, alg, stats );
  endTime = clock();
  logEvent( END_TEST, 0 );
  stats->end_cycles = endTime;
  printf("\n\n~~~~~~~~~Done with simulation at %lu~~~~~~~\n", endTime);
  return;
}

int destroyWorkLoad(Workload* wl, int testSize) {
  int i;
  for (i = 0; i < testSize; i++) {
    free(wl->tasks[i]);
  }
  free(wl->tasks);
  return 1; //could be void, I guess
}

void displayStats(Stats* stats, int testSize){
  int i;
  for (i = 0; i < testSize; i++) {
    printf("[%d]\ndeadlines missed:%d\nexecuted cycles:%lu\nexecution number:%d\n",
		i,(stats->task_stats[i])->deadlines_missed,
		(stats->task_stats[i])->exec_cycles,
		(stats->task_stats[i])->exec_number);
  }
  printf("\tOverall deadlines missed:%d\n",stats->total_deadlines_missed);
  return;
}

int destroyStats(Stats* stats, int testSize){
  int i;
  for (i = 0; i < testSize; i++) {
    free(stats->task_stats[i]);
  }
  free(stats->task_stats);
  return 1;
}

void TaskThread(void *arguments) {
  TaskArguments *args = (TaskArguments *) arguments;
  int threadId = args->thread_id;
  int spinTime = args->exec_time_us;

  //MsgRecieve
  spin(spinTime);
  //MsgReply
  return;
}

pthread_t *initTaskThread(int threadId, int execTimeUs) {
  TaskArguments *args;
  args = malloc(sizeof(TaskArguments));
  args->thread_id = threadId;
  args->exec_time_us = execTimeUs;
  //TODO ? return initThread( 0, (void *)TaskThread, (void *)args);
  return 0;
}

