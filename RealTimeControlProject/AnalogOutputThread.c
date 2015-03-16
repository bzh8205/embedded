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
#include <errno.h>      // defines EOK
#include <sys/neutrino.h> //MsgReceive and MsgReply
#include "AnalogOutputThread.h"
#include "GeneralUtils.h"
#include "ThreadMsg.h"

void AnalogOutputThread(void *arguments) {
  int rcvid;
  ThreadMessage message;
  AnalogOutputThreadArgs *args = (AnalogOutputThreadArgs *) arguments;
  int chid = args->ch_id;
  int threadId = args->thread_id;
  printf("AnalogOutputThread %d created\n", threadId);

  //run until message signals to exit
  while( 1 ){
    rcvid = MsgReceive(chid, &message, sizeof(message), NULL);
    if( message.exit != 1 ){
      //TODO read analog input
      message.value = message.value * 2;
      MsgReply( rcvid, EOK, &message, sizeof(message) );
    } else {
      printf("AnalogOutputThread %d exiting\n", threadId);
      MsgReply( rcvid, EOK, &message, sizeof(message) );
      break;
    }
  }

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
