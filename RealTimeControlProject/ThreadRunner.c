/*
 * ThreadRunner.c
 *
 *  Created on: Mar 11, 2015
 *      Author: Brian
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/neutrino.h> //channel creatte
#include <pthread.h>    // pthread_create/exit
#include <sys/netmgr.h> // channel attach
#include "ThreadRunner.h"
#include "AnalogInputThread.h"
#include "AnalogOutputThread.h"
#include "ControlCalculationThread.h"
#include "UserInputThread.h"
#include "ThreadMsg.h"

static pthread_t* analogInputThread;
static pthread_t* analogOutputThread;
static pthread_t* userInputThread;
static pthread_t* controlCalcThread;
static int A_IN_THREAD_ID = 0;
static int A_OUT_THREAD_ID = 1;
static int U_IN_THREAD_ID = 2;
static int CONTROL_THREAD_ID = 3;
static int THREAD_IDS[] = { 0, 1, 2, 3 };
static int CHANNEL_IDS[4];
static int CONNECTION_IDS[4];
static int NUM_THREADS = 4;

void startThreads(){
  int iter;
  //threads need channels to receive messages
  printf("Creating channels\n");
  for( iter = 0 ; iter < NUM_THREADS ; iter++ ){
    CHANNEL_IDS[iter] = ChannelCreate (0);
  }

  //we need connections to send messages to threads
  printf("Creating connections\n");
  for( iter = 0 ; iter < NUM_THREADS ; iter++ ){
    CONNECTION_IDS[iter] = ConnectAttach(ND_LOCAL_NODE, 0, CHANNEL_IDS[iter], THREAD_IDS[iter], 0);
  }

  //create the threads and pass them their channel for messages
  analogInputThread = initAnalogInputThread( CHANNEL_IDS[A_IN_THREAD_ID], A_IN_THREAD_ID);
  analogOutputThread = initAnalogOutputThread( CHANNEL_IDS[A_OUT_THREAD_ID], A_OUT_THREAD_ID);
  userInputThread = initUserInputThread( CHANNEL_IDS[U_IN_THREAD_ID], U_IN_THREAD_ID);
  controlCalcThread = initControlCalculationThread( CHANNEL_IDS[CONTROL_THREAD_ID], CONTROL_THREAD_ID);

  printf("Thread Runner: waiting for user input to start.\n");
  while( USER_IN_RUN_THREADS == 0 ){ }

  if( USER_IN_RUN_THREADS == 1 ) {

    printf("Thread Runner: starting PID loop.\n");

    //Tests messaging chain
    ThreadMessage msg = { 0 , 0 };
    int reply_status;

    while( USER_IN_RUN_THREADS ){
      reply_status = MsgSend(CONNECTION_IDS[A_IN_THREAD_ID], &msg, sizeof(msg), &msg, sizeof(msg));
      printf("Analog Input result: %f\n", msg.value);
      reply_status = MsgSend(CONNECTION_IDS[CONTROL_THREAD_ID], &msg, sizeof(msg), &msg, sizeof(msg));
      printf("Control Calculation result: %f\n", msg.value);
      reply_status = MsgSend(CONNECTION_IDS[A_OUT_THREAD_ID], &msg, sizeof(msg), &msg, sizeof(msg));
      printf("Analog Output result: %f\n", msg.value);
    }
  }

  if( USER_IN_RUN_THREADS == -1 ){
    printf("Thread Runner: Ending threads.\n");
    endThreads();
  }
}

void endThreads(){
  printf("Ending threads\n");
  //this will tell each thread that it needs to exit
  ThreadMessage msg = { 1 , 0 };
  int replyStatus;
  replyStatus = MsgSend(CONNECTION_IDS[A_IN_THREAD_ID], &msg, sizeof(msg), &msg, sizeof(msg));
  replyStatus = MsgSend(CONNECTION_IDS[CONTROL_THREAD_ID], &msg, sizeof(msg), &msg, sizeof(msg));
  replyStatus = MsgSend(CONNECTION_IDS[A_OUT_THREAD_ID], &msg, sizeof(msg), &msg, sizeof(msg));

  int iter;
  printf("Destroying channels\n");
  for( iter = 0 ; iter < NUM_THREADS ; iter++ ){
    ChannelDestroy(CHANNEL_IDS[iter]);
  }
}
