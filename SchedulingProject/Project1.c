//TODO resolve imports
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "BurnUtility.h"
#include "Project1.h"

//example workload descriptor
unsigned int test1[5][3] = {{1,7,7}, {2,5,5}, {1,8,8}, {1,10,10}, {2,16,16}};
int test1Size = 5;


int main(int argc, char *argv[]) {
  Workload wl;
//   	int i;
  fudge =1; //defaulting fudge variable before test adjustment
  //TODOinitBurnUtility();
  initSpinUtility();
  initSpinUtility();
  //printf("Fudge: %f\n",fudge);
  //create workload struct from descriptor
/*
  if (!initWorkLoad(&wl,(unsigned int **)test1,test1Size) ) {
	printf("Error initializing Workload struct\n");
  }
//check
	for (i=0;i<test1Size;i++) {
		printf("id:%d C:%d P:%d D:%d LF:%d ND:%d\n", (wl.tasks[i])->id,
			(wl.tasks[i])->exec_time_us , (wl.tasks[i])->period_time_us , 
			(wl.tasks[i])->deadline_us ,
			(wl.tasks[i])->last_finish_us, (wl.tasks[i])->next_deadline_us );
	}
  */
	//create threads
  
  //runTest( Workload, configuration ) -> returns [event data]
  //createStats( [event data] ) -> returns [stats struct]
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
int initWorkLoad(Workload* wl, unsigned int** test, int testSize ){
	int i;
	Task** tptr = malloc(sizeof(Task*)*testSize);//create task ptr array
	for (i=0;i<testSize; i++) {
		tptr[i] = malloc(sizeof(Task)); // create task structure
printf("created struct %d in task array\n",i);
		tptr[i]->id =i; 
printf("exe time:%d\n", test[i][0]); //TODO fix test infrasture referencing
		tptr[i]->exec_time_us = test[i][1];
		tptr[i]->period_time_us = test[i][2];
		tptr[i]->deadline_us = test[i][3];
		tptr[i]->last_finish_us = 0; //default
		tptr[i]->next_deadline_us = 0; //default
printf("done w/ struct %d\n", i);
	}
printf("we're ok?\n");
	//add to struct
	wl->task_num = testSize;
	wl->tasks = tptr;


//testing again....
for (i=0;i<test1Size;i++) {
                printf("Tasks array:: id:%d C:%d P:%d D:%d LF:%d ND:%d\n", (tptr[i])->id,
                        (tptr[i])->exec_time_us , (tptr[i])->period_time_us ,
                        (tptr[i])->deadline_us ,
                        (tptr[i])->last_finish_us, (tptr[i])->next_deadline_us );
        }
for (i=0;i<test1Size;i++) {
                printf("WL ref Tasks array:: id:%d C:%d P:%d D:%d LF:%d ND:%d\n", (wl->tasks[i])->id,
                        (wl->tasks[i])->exec_time_us , (wl->tasks[i])->period_time_us ,
                        (wl->tasks[i])->deadline_us ,
                        (wl->tasks[i])->last_finish_us, (wl->tasks[i])->next_deadline_us );
        }
	return 1; 
}

//TODO: params and returns
void runTest() {
  //save start time
  //while time < configuration.test_duration
  //  id = scheduleTask();
  //  log start running id.
  //  MsgSend [to id]
  //  log end running id 
  //  update workload.tasks[id]
   return;
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

