#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/neutrino.h> //channel creatte
#include <sys/netmgr.h> // channel attach
#include <errno.h>      // defines EOK
#include <pthread.h>    // pthread_create/exit
#include <time.h>
#include "BurnUtility.h"
#include "ThreadRunner.h"
#include "Workload.h"

int RUN_REPLY = 0;
int EXIT_REPLY = 1;

int *rcvids_ptr;
int chid;
int numThreads;

//inits given thread with args
pthread_t *initThread( int rel_prio, void *thread, void *args ){
  pthread_t *pthread = malloc(sizeof(pthread_t));
  pthread_attr_t threadAttributes;
  int policy;
  struct sched_param parameters;

  pthread_attr_init(&threadAttributes) ;    // initialize thread attributes structure -- must do before any other activity on this struct
  pthread_getschedparam(pthread_self(), &policy, &parameters) ; // get this main thread's scheduler parameters
  parameters.sched_priority += rel_prio;  // adjust the priority
  parameters.sched_curpriority = parameters.sched_priority;
  printf("Creating thread with priority %d\n", parameters.sched_priority);
  pthread_attr_setschedparam(&threadAttributes, &parameters) ;  // set up the pthread_attr struct with the updated priority

  pthread_create( pthread, &threadAttributes, thread, args );
  return pthread;
}

void TaskThread(void *arguments) {
  TaskArguments *args = (TaskArguments *) arguments;
  int chid = args->ch_id;
  int threadId = args->thread_id;
  int spinTime = args->exec_time_us;
  int rmsg = 0;
  printf("Thread %d created\n", threadId);

  int coid = ConnectAttach(ND_LOCAL_NODE, 0, chid, threadId, 0);
  if( coid == -1 ){
    fprintf (stderr, "Couldn't ConnectAttach to 0/%d/%d!\n", chid, threadId);
    perror (NULL);
    exit (EXIT_FAILURE);
  }

  while (1){
    int reply = MsgSend(coid, &threadId, sizeof(threadId), &rmsg, sizeof(rmsg));
    if ( reply == -1 ){
      fprintf (stderr, "Error during MsgSend\n");
      perror (NULL);
      exit (EXIT_FAILURE);
    } else if ( rmsg == 0 ){
      printf("Spin for %d\n", spinTime);
      spin(spinTime);
    } else {
      break;
    }
  }

  printf("Thread %d finished\n", threadId);
  pthread_exit(0);
  return;
}

pthread_t *initTaskThread(int chid, int threadId, int execTimeUs) {
  TaskArguments *args;
  args = malloc(sizeof(TaskArguments));
  args->ch_id = chid;
  args->thread_id = threadId;
  args->exec_time_us = execTimeUs;
  return initThread( 0, (void *)TaskThread, (void *)args);
}

void initThreadRunner(Workload *workload){
  numThreads = workload->task_num;

  printf("Creating channel\n");
  chid = ChannelCreate (0);
  printf("Channel created with id %d\n", chid);

  int rcvids[numThreads];
  int iter; int message;
  for( iter = 0 ; iter < numThreads ; iter++ ){
    printf("Creating thread for task id %d, with exec_time %d",
        (workload->tasks[iter])->id, (workload->tasks[iter])->exec_time_us);
    initTaskThread( chid, iter, (workload->tasks[iter])->exec_time_us );
    rcvids[iter] = MsgReceive(chid, &message, sizeof(message), NULL );
  }

  rcvids_ptr = rcvids;
}

void runThread(int num){
  int message;
  MsgReply( rcvids_ptr[num], EOK, &RUN_REPLY, sizeof(RUN_REPLY) );
  rcvids_ptr[num] = MsgReceive(chid, &message, sizeof(message), NULL);
}

void destroyThreadRunner(){
  int iter;
  for( iter = 0 ; iter < numThreads ; iter++ ){
    MsgReply( rcvids_ptr[iter], EOK, &EXIT_REPLY, sizeof(EXIT_REPLY) );
  }
  printf("Waiting for threads to end...\n");
  sleep(1);
  printf("Destroying channel\n");
  ChannelDestroy(chid);
}
