//TODO resolve imports

#include "BurnUtility.h"

//example workload descriptor
unsigned int test1[][] = {{1,7,7}, {2,5,5}, {1,8,8}, {1,10,10}, {2,16,16}};

int main(int argc, char *argv[]) {
  initBurnUtility();
  //create workload struct from descriptor
  //create threads
  
  //runTest( Workload, configuration ) -> returns [event data]
  //createStats( [event data] ) -> returns [stats struct]
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
}

void TaskThread(void *arguments){
  TaskArguments *args = (TaskArguments *)arguments;
  int threadId = args->thread_id; 
  int spinTime = args->exec_time_us; 
  
  //MsgRecieve
  spin( spinTime );
  //MsgReply

}

pthread_t *initTaskThread( int threadId, int execTimeUs ){
  TaskArguments *args;
  args = malloc(sizeof(TaskArguments));
  args->thread_id = threadId;
  args->exec_time_us = execTimeUs;
  return initThread( 0, (void *)TaskThread, (void *)args);
}

