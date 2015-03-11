/*
 * ThreadUtils.c
 *
 *  Created on: Mar 11, 2015
 *      Author: Brian
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>    // pthread_create/exit

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
