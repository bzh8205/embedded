/*
 * UserInputThread.c
 *
 *  Created on: Mar 11, 2015
 *      Author: Brian
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>    // pthread_create/exit
#include "UserInputThread.h"
#include "ThreadUtils.h"

void UserInputThread(void *arguments) {
  UserInputThreadArgs *args = (UserInputThreadArgs *) arguments;
  int chid = args->ch_id;
  int threadId = args->thread_id;
  printf("UserInputThread %d created\n", threadId);
  printf("UserInputThread %d finished\n", threadId);
  pthread_exit(0);
  return;
}

pthread_t *initUserInputThread(int chid, int threadId) {
  UserInputThreadArgs *args;
  args = malloc(sizeof(UserInputThreadArgs));
  args->ch_id = chid;
  args->thread_id = threadId;
  return initThread( 0, (void *)UserInputThread, (void *)args);
}
