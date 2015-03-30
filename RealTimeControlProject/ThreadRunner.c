/*
 * ThreadRunner.c
 *
 *  Created on: Mar 11, 2015
 *      Author: Brian
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/neutrino.h> //channel creatte, pulses too
#include <pthread.h>    // pthread_create/exit
#include <sys/netmgr.h> // channel attach
#include <assert.h> //for timer create
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

//Creates channel and sends pulse with pulseid on it every nsfreq.
int InitializeTimer(long nsfreq, int pulseid){
  int chid;
  timer_t timer_id;
  struct sigevent event;
  struct itimerspec timer;
  printf("Initializing Pulses at %ldns with pulse id as %d\n", nsfreq, pulseid);

  /* Create a channel to receive timer events on. */
  chid = ChannelCreate( 0 ); //b: opening up a channel to send messages through
  assert ( chid != -1 );      // if returns a -1 for failure we stop with error
  /* Set up the timer and timer event. */
  event.sigev_notify = SIGEV_PULSE;   // most basic message we can send -- just a pulse number
  event.sigev_coid = ConnectAttach ( ND_LOCAL_NODE, 0, chid, 0, 0 );  // Get ID that allows me to communicate on the channel
  assert ( event.sigev_coid != -1 );    // stop with error if cannot attach to channel
  event.sigev_priority = getprio(0);
  event.sigev_value.sival_int = pulseid;

  // Now create the timer and get back the timer_id value for the timer we created.
  if ( timer_create( CLOCK_REALTIME, &event, &timer_id ) == -1 )  // CLOCK_REALTIME available in all POSIX systems
  {
    perror ( "cannot create timer\n" );
    exit( EXIT_FAILURE );
  }

  /* Change the timer request to alter the behavior. */
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_nsec = nsfreq;    // interrupt at .1 ms.
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_nsec = nsfreq; // keep interrupting every .1 ms.

  /* Start the timer. */
  if ( timer_settime( timer_id, 0, &timer, NULL ) == -1 )
  {
    perror("cannot start timer.\n");
    exit( EXIT_FAILURE );
  }

  return chid;
}

void startThreads(){
  struct _pulse pulse; //for timer
  int iter;
  int chid, pid; //for timer
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
    chid = InitializeTimer(1000000, 1); //set timer to 10hz or 100ms

    //Tests messaging chain
    ThreadMessage msg = { 0 , 0 };
    int reply_status;

    while( USER_IN_RUN_THREADS ){
      pid = MsgReceivePulse ( chid, &pulse, sizeof( pulse ), NULL );
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


