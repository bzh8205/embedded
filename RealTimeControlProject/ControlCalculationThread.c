/*
 * ControlCalculationThread.c
 *
 *  Created on: Mar 11, 2015
 *      Author: Brian
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>    // pthread_create/exit
#include <errno.h>      // defines EOK
#include <sys/neutrino.h> //MsgReceive and MsgReply
#include "ControlCalculationThread.h"
#include "GeneralUtils.h"
#include "ThreadMsg.h"

static float K_P = 0;
static float K_I = 0;
static float K_D = 0;

void ControlCalculationThread(void *arguments) {
  int rcvid;
  ThreadMessage message;
  ControlCalculationThreadArgs *args = (ControlCalculationThreadArgs *) arguments;
  int chid = args->ch_id;
  int threadId = args->thread_id;
  printf("ControlCalculationThread %d created\n", threadId);

  int z0 = 0;
  int z1 = 0;
  int z2 = 0;

  //run until message signals to exit
  while( 1 ){
    rcvid = MsgReceive(chid, &message, sizeof(message), NULL);
    if( message.exit != 1 ){
      //update z variables... this might not be a thing...
      z0 = z1;
      z1 = z2;
      z2 = message.value;

      //calculate control output
      // [(Kp+Ki+Kd)*z^2 - (Kp+2Kd)*z + Kd] divided by [(z-1)z]
      message.value = ( ((K_P + K_I + K_D)*message.value*message.value)
          - ((K_P + K_D + K_D)*message.value) + (K_D))
          / ((message.value - 1) * message.value);
      MsgReply( rcvid, EOK, &message, sizeof(message) );
    } else {
      printf("ControlCalculationThread %d exiting\n", threadId);
      MsgReply( rcvid, EOK, &message, sizeof(message) );
      break;
    }
  }

  printf("ControlCalculationThread %d finished\n", threadId);
  pthread_exit(0);
  return;
}

void setPIDConstants( float Kp, float Ki, float Kd ){
  K_P = Kp;
  K_I = Ki;
  K_D = Kd;
}

pthread_t *initControlCalculationThread(int chid, int threadId) {
  ControlCalculationThreadArgs *args;
  args = malloc(sizeof(ControlCalculationThreadArgs));
  args->ch_id = chid;
  args->thread_id = threadId;
  return initThread( 0, (void *)ControlCalculationThread, (void *)args);
}
