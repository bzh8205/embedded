//TODO resolve imports
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/trace.h>
#include <sys/neutrino.h>
#include "BurnUtility.h"
#include "ThreadRunner.h"
#include "UserTracing.h"
#include "Scheduler.h"
#include "Project1.h"

/** ~~~~~~COMPILER FLAGS~~~~~~~~~~~~~~*/
//#define CONSOLE_PRINT
//#define TEST2			//running 2nd task set
//#define TEST3			//running 3rd task set
				//default 1st task set
//#define EDF_TEST		//running test using EDF scheduler
//#define LST_TEST		//running test using LST scheduler
				//default Rate Mon

//util variables
unsigned char is_time_init = 0;
struct timespec init_time;

/*~~~~~~~~ Task Workload Tests~~~~~~~~~~~~~~~~*/
#ifdef TEST3
unsigned int test1[4][3] = { {1,4,4}, {2,5,5}, {1,8,8}, {1,10,10}};
int test1Size = 4;
#elif defined(TEST2)
//lazy test switching
unsigned int test1[3][3]= { {1,3,3}, {2,5,5}, {1,10,10}};
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
  initSpinUtility();
  spinTest();
#ifndef ALYSSA_TESTING
  initUserTracing(argv[0]);
#endif
  //printf("Fudge: %f\n",fudge);
  //create workload struct from descriptor

  if (!initWorkLoad(&wl, test1, test1Size)) {
    printf("Error initializing Workload struct\n");
  }

  initThreadRunner(&wl);

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

  //Select Scheduling algorithm to benchmark
#ifdef EDF_TEST
  runTest(&wl, EARLIEST_DEADLINE, &stats);
#elif defined(LST_TEST)
  runTest(&wl, LEAST_SLACK, &stats);
#else
  runTest(&wl, LEAST_SLACK, &stats);
#endif
//#ifdef ALYSSA_TESTING
  //print out the stats
  displayStats(&stats,test1Size);
//#endif
  //free stats
  destroyStats(&stats, test1Size);

  //free workload
  destroyWorkLoad(&wl, test1Size);

  spinTest();
  return 0;
}


//TODO added new time funct
//On first call: sets up seconds to be calculated from first call time,
//rather than 1970. Else: returns time from first call.
long getTime(){
	struct timespec current;
	long result, seconds, milliseconds;
	if( is_time_init == 0 ){
		if( clock_gettime( CLOCK_REALTIME, &init_time ) == -1 ){
			perror("clock_gettime");
		}
		is_time_init = 1;
		result = 0L;
	} else {
		if( clock_gettime( CLOCK_REALTIME, &current ) == -1 ){
			perror("clock_gettime");
		}

		seconds = (long)current.tv_sec - init_time.tv_sec;
		milliseconds = (long)((current.tv_nsec - init_time.tv_nsec)/1000000);
		result = seconds*1000 + milliseconds;

		//result = (long)( ((current.tv_sec - init_time.tv_sec) * 1000) +
		//		((current.tv_nsec - init_time.tv_nsec)/100000) );
	}
	return result;
}

long getTimeUs(){
  struct timespec current;
    long result, seconds, microseconds;
    if( is_time_init == 0 ){
      if( clock_gettime( CLOCK_REALTIME, &init_time ) == -1 ){
        perror("clock_gettime");
      }
      is_time_init = 1;
      result = 0L;
    } else {
      if( clock_gettime( CLOCK_REALTIME, &current ) == -1 ){
        perror("clock_gettime");
      }

      seconds = (long)current.tv_sec - init_time.tv_sec;
      microseconds = (long)((current.tv_nsec - init_time.tv_nsec)/1000);
      result = seconds*1000000 + microseconds;

      //result = (long)( ((current.tv_sec - init_time.tv_sec) * 1000) +
      //    ((current.tv_nsec - init_time.tv_nsec)/100000) );
    }
    return result;
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
    tptr[i]->exec_time_us = test[i][0] * FACTOR;
    tptr[i]->period_time_us = test[i][1] * FACTOR;
    tptr[i]->deadline_us = test[i][2] * FACTOR;
    tptr[i]->last_finish_us = 0; //default
    tptr[i]->next_deadline_us = test[i][2] * FACTOR; //default TODO remove x10
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
  stats->start_time_ms = 0;//clock; //TODO
  stats->end_time_ms = 0;
  stats->idle_time_us = 0;
  stats->exec_time_us = 0;
  stats->exec_num = 0;
  stats->idle_num = 0;
  stats->total_deadlines_missed = 0;//; //TODO

  TaskStats ** ts_ptr = malloc(sizeof(TaskStats*) * task_num);
  for (i = 0; i < task_num; i++) {
    ts_ptr[i] = malloc(sizeof(TaskStats));
    ts_ptr[i]->exec_time_us = 0;
    ts_ptr[i]->exec_number = 0;
    ts_ptr[i]->deadlines_missed = 0;
  }
  stats->task_stats = ts_ptr;

  return 1;
}

//Updates idle time if taskId = -1, updates a task stats otherwise.
void updateStats(int taskId, Workload* wl, long startus, long endus,
    Stats* stats) {
  if (taskId == -1 && (startus/1000 > stats->start_time_ms)) {
    stats->idle_time_us = (endus - startus);
    stats->idle_num++;
  } else if (taskId != -1) { //checking for non-neg index, led to segfault
    stats->exec_time_us += (endus - startus);
    stats->exec_num++;
    (stats->task_stats[taskId])->exec_time_us += (endus - startus);
    (stats->task_stats[taskId])->exec_number += 1;
  }
}

void logEvent(EVENT_TYPE et, int info) {
  userTraceEvent((int)et, info);

#ifdef CONSOLE_PRINT
  switch(et){
  case SCHED_START:
    printf("SCHEDULING A TASK info %d time %lu\n", info, getTime());
    break;
  case NOTHING_SCHED:
    printf("NO TASK SCHEDULED info %d time %lu\n", info, getTime());
    break;
  case TASK_SCHED:
    printf("TASK SCHEDULED info %d time %lu\n", info, getTime());
    break;
  case TASK_EXEC_START:
    printf("TASK EXECUTION START info %d time %lu\n", info, getTime());
    break;
  case TASK_EXEC_END:
    printf("TASK EXECUTION END info %d time %lu\n", info, getTime());
    break;
  case START_TEST:
    printf("TEST START info %d time %lu\n", info, getTime());
    break;
  case END_TEST:
    printf("TEST END info %d time %lu\n", info, getTime());
    break;
  case DEADLINE_MISSED:
    printf("DEADLINE MISSED info %d time %lu\n", info, getTime());
    break;
  }
#endif
}

void printTaskInfo(Task* t) {
  printf("[%d] C:%d P:%d D:%d LE:%d LF:%d ND:%d\n", t->id, t->exec_time_us,
      t->period_time_us, t->deadline_us, t->last_exec_us, t->last_finish_us,
      t->next_deadline_us);
}

/**
 * \brief Updates the next deadline if deadlines were missed and not automatically
 * updated after execution
 */
void updateDeadlines(long lastClock, Workload* wl,Stats * stats) {
   int id;
   for (id=0 ; id< wl->task_num; id++) {
	//TODO, error with comparing this inequality
      if ( lastClock > (unsigned long) (wl->tasks[id])->next_deadline_us ) { //if DL passed 
//        printf("ceil: %f of %lu/%u=%f\n", ceil( (double) lastClock/(wl->tasks[id])->deadline_us),
//            lastClock, (wl->tasks[id])->deadline_us,(double) lastClock/(wl->tasks[id])->deadline_us);
        (wl->tasks[id])->next_deadline_us = (unsigned int)
            ceil ( (double)lastClock/(wl->tasks[id])->deadline_us) * (wl->tasks[id])->deadline_us;


      //update the deadline missed
      //printf("UD[%d]:%d\n",id, (wl->tasks[id])->next_deadline_us);
      LogEvent( DEADLINE_MISSED, id );
      (stats->task_stats[id])->deadlines_missed += 1;
      stats->total_deadlines_missed += 1;
    }
  }
  return;
}


void _runTest(clock_t startTime, Workload* wl, SCHED_ALG alg, Stats* stats){
  int id;
  long pre_exec = 0;
  long post_exec = 0;
  int sched_ctr = 0;
  long tick=0;
  //while time < configuration.test_duration
  tick = getTime();
  while (tick < startTime + 200*FACTOR) {//TODO remove FACTOR
    updateDeadlines(tick-startTime, wl,stats);
   //TODO not updating fast enough, times between execution too costly
    sched_ctr++;
    logEvent( SCHED_START, sched_ctr );
    id = scheduleTask(wl, alg,tick);
    if (id != -1) {
      logEvent(TASK_SCHED, id);
      //LOG: start task spin
      pre_exec = getTimeUs(); //TODO by this time first deadline has passed!
//#ifndef ALYSSA_TESTING
      updateStats(-1, wl, post_exec, pre_exec, stats);
      //#endif
      //printf("starting task %d at: %lu\n",id,pre_exe);
      logEvent(TASK_EXEC_START, id);
      runThread(id);
      //spin((wl->tasks[id])->exec_time_us*1000);
      logEvent(TASK_EXEC_END, id);
      //LOG: end task spin
      post_exec = getTimeUs();
      //#ifdef ALYSSA_TESTING
      updateStats(id, wl, pre_exec, post_exec, stats);
      //#endif
      //update workload
      (wl->tasks[id])->last_finish_us = post_exec/1000-startTime;
      (wl->tasks[id])->last_exec_us = pre_exec/1000-startTime;
      (wl->tasks[id])->next_deadline_us +=(wl->tasks[id])->deadline_us;
//#ifdef ALYSSA_TESTING
     // printf("[%d]\n",id);
#ifdef CONSOLE_PRINT
        printf("%lu\n",tick-startTime);
        printTaskInfo( (wl->tasks[id]) );
#endif
//#endif
      //bad for wiggle spins
      //printf("took %lu\n", post_exe - pre_exe);
    } else {
//#ifdef ALYSSA_TESTING
#ifdef CONSOLE_PRINT
      printf("Nothing Scheduled\n");
      //updateDeadlines(getTime(), wl);
      printf("%lu\n",tick-startTime);
#endif
//#endif
      logEvent( NOTHING_SCHED, 0 );
    }
    tick = getTime(); //next tick
  } //done test
}

//TODO: params and returns
void runTest(Workload* wl, SCHED_ALG alg, Stats* stats) {
  long startTime, endTime;
  //save start time
  startTime = getTime();
  logEvent( START_TEST, 0 );
  stats->start_time_ms = startTime;
  printf("~~~~~~~~~~Starting Sim at %lu~~~~~~~~~~~~\n", startTime);
  _runTest( startTime, wl, alg, stats );
  endTime = getTime();
  logEvent( END_TEST, 0 );
  stats->end_time_ms = endTime;
  printf("\n~~~~~~~~~Done with simulation at %lu~~~~~~~\n", endTime);
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

void displayStats(Stats* stats, int testSize) {
  int i;
  for (i = 0; i < testSize; i++) {
    printf(
        "[%d]\ndeadlines missed: %d\nexecuted time ms: %lu\nexecution number: %d\nexecution avg ms: %f\n",
        i, (stats->task_stats[i])->deadlines_missed,
        (stats->task_stats[i])->exec_time_us/1000,
        (stats->task_stats[i])->exec_number,
        (stats->task_stats[i])->exec_time_us/1000.0/stats->task_stats[i]->exec_number);
  }
  printf("\tStart time ms: %d\n", stats->start_time_ms);
  printf("\tEnd time ms: %d\n", stats->end_time_ms);
  printf("\tTotal time ms: %d\n", (stats->end_time_ms)-(stats->start_time_ms));
  printf("\tIdle time us: %lu\n", (stats->idle_time_us));
  printf("\tIdle num: %d\n", (stats->idle_num));
  printf("\tExec time us: %lu\n", (stats->exec_time_us));
  printf("\tExec num: %d\n", (stats->exec_num));
  printf("\tIdle time average ms: %f\n", (stats->idle_time_us)/1000.0/(stats->idle_num));
  printf("\tExec time average ms: %f\n", (stats->exec_time_us)/1000.0/(stats->exec_num));
  printf("\tCPU Utilization: %f\n", (stats->exec_time_us)/1000.0/((stats->end_time_ms)-(stats->start_time_ms)));
  printf("\tOverall deadlines missed: %d\n", stats->total_deadlines_missed);
  return;
}

int destroyStats(Stats* stats, int testSize) {
  int i;
  for (i = 0; i < testSize; i++) {
    free(stats->task_stats[i]);
  }
  free(stats->task_stats);
  return 1;
}
