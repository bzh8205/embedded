//TODO resolve imports
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "BurnUtility.h"
#include "Scheduler.h"
#include "Project1.h"

//example workload descriptor
unsigned int test1[5][3] = {{1,7,7}, {2,5,5}, {1,8,8}, {1,10,10}, {2,16,16}};
int test1Size = 5;


int main(int argc, char *argv[]) {
  Workload wl;
   int i;
  fudge =1; //defaulting fudge variable before test adjustment
  //TODOinitBurnUtility();
  initSpinUtility();
  //printf("Fudge: %f\n",fudge);
  //create workload struct from descriptor

  if (!initWorkLoad(&wl,test1,test1Size) ) {
	printf("Error initializing Workload struct\n");
  }
//check
	for (i=0;i<test1Size;i++) {
		printf("id:%d C:%d P:%d D:%d LF:%d ND:%d\n", (wl.tasks[i])->id,
			(wl.tasks[i])->exec_time_us , (wl.tasks[i])->period_time_us , 
			(wl.tasks[i])->deadline_us ,
			(wl.tasks[i])->last_finish_us, (wl.tasks[i])->next_deadline_us );
	}
  
	//create threads
  
  //runTest( Workload, configuration ) -> returns [event data]
  runTest(&wl,EARLIEST_DEADLINE);
  //runTest(&wl,LEAST_SLACK);


  //createStats( [event data] ) -> returns [stats struct]

	//free workload
	destroyWorkLoad(&wl,test1Size);
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
int initWorkLoad(Workload* wl, unsigned int test[][3], int testSize ){
	int i;
	Task** tptr = malloc(sizeof(Task*)*testSize);//create task ptr array
	for (i=0;i<testSize; i++) {
		tptr[i] = malloc(sizeof(Task)); // create task structure
//printf("created struct %d in task array\n",i);
		tptr[i]->id =i; 
//printf("exe time:%d\n", test[i][0]); //TODO fix test infrasture referencing
		tptr[i]->exec_time_us = test[i][0];
		tptr[i]->period_time_us = test[i][1];
		tptr[i]->deadline_us = test[i][2];
		tptr[i]->last_finish_us = 0; //default
		tptr[i]->next_deadline_us = test[i][2]; //default
//printf("done w/ struct %d\n", i);
	}
	//add to struct
	wl->task_num = testSize;
	wl->tasks = tptr;

	return 1; 
}

//TODO: params and returns
void runTest(Workload* wl,SCHED_ALG alg) {
	unsigned int id; 
	clock_t startTime,pre_exe,post_exe,endTime;
  //save start time
	startTime = clock();
	printf("~~~~~~~~~~Starting Sim at %lu~~~~~~~~~~~~\n\n",startTime);
  //while time < configuration.test_duration
	while (clock() < startTime+200) {
		id = scheduleTask(wl,alg);
		if (id != -1) {
  //  log start running id.
			pre_exe = clock();
		//printf("starting task %d at: %lu\n",id,pre_exe);
  //  MsgSend [to id]
			spin((wl->tasks[id])->exec_time_us);	
  //  log end running id 
			post_exe = clock();
			(wl->tasks[id])->last_finish_us = post_exe;
			(wl->tasks[id])->next_deadline_us= pre_exe+(wl->tasks[id])->deadline_us; //deadline marks from start exe
			//bad for wiggle spins
			
			printf("ending task %d at: %lu\n",id,post_exe);
			printf("took %lu\n",post_exe - pre_exe);
  //  update workload.tasks[id]
		}else {
			printf("No tasks are ready\n");
		}
	} //done test
	endTime=clock();
	printf("\n\n~~~~~~~~~Done with simulation at %lu~~~~~~~\n",endTime);
   return;
}


int destroyWorkLoad(Workload* wl,int testSize ) {
	int i;
        for (i=0;i<testSize; i++) {
		free(wl->tasks[i]);
        }
	free(wl->tasks);
	return 1; //could be void, I guess
}


void TaskThread(void *arguments){
  TaskArguments *args = (TaskArguments *)arguments;
  int threadId = args->thread_id; 
  int spinTime = args->exec_time_us; 
  
  //MsgRecieve
  spin( spinTime );
  //MsgReply
  return;
}

pthread_t *initTaskThread( int threadId, int execTimeUs ){
  TaskArguments *args;
  args = malloc(sizeof(TaskArguments));
  args->thread_id = threadId;
  args->exec_time_us = execTimeUs;
 //TODO ? return initThread( 0, (void *)TaskThread, (void *)args);
  return 0; 
}

