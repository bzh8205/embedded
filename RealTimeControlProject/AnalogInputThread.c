/*
 * AnalogInputThread.c
 *
 *  Created on: Mar 11, 2015
 *      Author: Brian
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>    // pthread_create/exit
#include "AnalogInputThread.h"
#include "GeneralUtils.h"

void AnalogInputThread(void *arguments) {
  AnalogInputThreadArgs *args = (AnalogInputThreadArgs *) arguments;
  int chid = args->ch_id;
  int threadId = args->thread_id;
  printf("AnalogInputThread %d created\n", threadId);
  printf("AnalogInputThread %d finished\n", threadId);
  pthread_exit(0);
  return;
}

pthread_t *initAnalogInputThread(int chid, int threadId) {
  AnalogInputThreadArgs *args;
  args = malloc(sizeof(AnalogInputThreadArgs));
  args->ch_id = chid;
  args->thread_id = threadId;
  return initThread( 0, (void *)AnalogInputThread, (void *)args);
}

