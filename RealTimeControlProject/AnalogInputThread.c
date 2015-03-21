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
#include <errno.h>      // defines EOK
#include <sys/neutrino.h> //MsgReceive and MsgReply
#include "AnalogInputThread.h"
#include "GeneralUtils.h"
#include "ThreadMsg.h"
#include "ADC.h"    //for adc mapping and reading

void AnalogInputThread(void *arguments) {
  int rcvid;
  ThreadMessage message;
  AnalogInputThreadArgs *args = (AnalogInputThreadArgs *) arguments;
  int chid = args->ch_id;
  int threadId = args->thread_id;
  printf("AnalogInputThread %d created\n", threadId);
  //TODO init ADC
  if ( ! GetRootAccess() ){
    SetupAtoD();
    //run until message signals to exit
    while( 1 ){
      rcvid = MsgReceive(chid, &message, sizeof(message), NULL);
      if( message.exit != 1 ){
        //TODO read analog input
        message.value = MeasureVoltageOnChannel( 1 ); //digital reading from ADC on ch 1
        message.value =((float)message.value / AD_SCALE) * INPUT_RANGE; //convert to volts
        MsgReply( rcvid, EOK, &message, sizeof(message) );
      } else {
        printf("AnalogInputThread %d exiting\n", threadId);
        MsgReply( rcvid, EOK, &message, sizeof(message) );
        break;
      }
    }
  }else {
    printf("AnalogInputThread::Couldn't get root access for mapping ADC\n");
  }


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

