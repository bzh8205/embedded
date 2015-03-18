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
#include "ADC.h"    //for DAC mapping and writing

void AnalogOutputThread(void *arguments) {
  int rcvid;
  ThreadMessage message;
  AnalogOutputThreadArgs *args = (AnalogOutputThreadArgs *) arguments;
  int chid = args->ch_id;
  int threadId = args->thread_id;
  printf("AnalogOutputThread %d created\n", threadId);

  //Init DAC
  if (! GetRootAccess() ){ //got root
    SetupAout();

    //run until message signals to exit
    while( 1 ){
      rcvid = MsgReceive(chid, &message, sizeof(message), NULL);
      if( message.exit != 1 ){
        //Write analog output
        if ( (message.value >=-10) & (message.value <= 10) ) { 
          GenerateAout (message.value,0); //output received value to ch 0
        } else {
            printf("AnalogOutputThread:: %fV is not a ranged value, no output\n",message.value);
        }
        MsgReply( rcvid, EOK, &message, sizeof(message) );
        
      } else {
        printf("AnalogOutputThread %d exiting\n", threadId);
        MsgReply( rcvid, EOK, &message, sizeof(message) );
        break;
      }
    }
  } else { //no root access
    printf("AnalogOutputThread::Couldn't get root access for mapping DAC\n");
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
