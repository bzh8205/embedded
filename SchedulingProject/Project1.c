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
  //TODOinitBurnUtility();
  initSpinUtility();
  printf("Fudge: %f\n",fudge);
  //create workload struct from descriptor
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
int initWorkLoad(Workload* wl, unsigned int ** test, int testSize ){
    //TODO
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

