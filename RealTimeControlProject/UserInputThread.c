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
#include "ControlCalculationThread.h"
#include "GeneralUtils.h"


void UserInputThread(void *arguments) {
  UserInputThreadArgs *args = (UserInputThreadArgs *) arguments;
  int chid = args->ch_id;
  int threadId = args->thread_id;
  printf("UserInputThread %d created\n", threadId);

  printf("Enter \"Kp Ki Kd\" constants separated by spaces (i.e.) \"2.0 3.5 1.2\".\n Press only enter to exit\n");

  float Kp;
  float Ki;
  float Kd;

  while( USER_IN_RUN_THREADS == 0 ) {
    getPIDInput(&Kp, &Ki, &Kd);
  }

  while ( USER_IN_RUN_THREADS == 1 ){
    getPIDInput(&Kp, &Ki, &Kd);
  }

  printf("UserInputThread: Exiting.\n");
  printf("UserInputThread %d finished\n", threadId);
  pthread_exit(0);
  return;
}

void getPIDInput( float *Kp, float *Ki, float *Kd){
  int scan;
  printf("Kp Ki Kd: ");
  scan = scanf("%f %f %f", Kp, Ki, Kd);
  printf("\n");
  if( scan == 3 ) {
    setPIDConstants(*Kp, *Ki, *Kd);
    printf("UserInputThread: PID constants set.\n");
    USER_IN_RUN_THREADS = 1;
  } else if ( scan == EOF ){
    USER_IN_RUN_THREADS = -1;
  } else {
    printf("UserInputThread: Invalid input, please enter 3 decimals seperated by spaces (i.e.) \"2.0 3.5 1.2\".\n");
  }
}

pthread_t *initUserInputThread(int chid, int threadId) {
  UserInputThreadArgs *args;
  args = malloc(sizeof(UserInputThreadArgs));
  args->ch_id = chid;
  args->thread_id = threadId;
  return initThread( 0, (void *)UserInputThread, (void *)args);
}
