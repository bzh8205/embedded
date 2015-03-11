/*
 * AnalogOutputThread.c
 *
 *  Created on: Mar 11, 2015
 *      Author: Brian
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>    // pthread_create/exit
#include "AnalogOutputThread.h"
#include "ThreadUtils.h"

void AnalogOutputThread(void *arguments) {
  AnalogOutputThreadArgs *args = (AnalogOutputThreadArgs *) arguments;
  int chid = args->ch_id;
  int threadId = args->thread_id;
  printf("AnalogOutputThread %d created\n", threadId);
  printf("AnalogOutputThread %d finished\n", threadId);
  pthread_exit(0);
  return;
}

pthread_t *initAnalogOutputThread(int chid, int threadId) {
  AnalogOutputThreadArgs *args;
  args = malloc(sizeof(AnalogOutputThreadArgs));
  args->ch_id = chid;
  args->thread_id = threadId;
  return initThread( 0, (void *)AnalogOutputThread, (void *)args);
}
