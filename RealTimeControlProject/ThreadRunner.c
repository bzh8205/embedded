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
#include "ThreadRunner.h"
#include "AnalogInputThread.h"
#include "AnalogOutputThread.h"
#include "ControlCalculationThread.h"
#include "UserInputThread.h"

static int chid;

static pthread_t* analogInputThread;
static pthread_t* analogOutputThread;
static pthread_t* userInputThread;
static pthread_t* controlCalcThread;

void startThreads(){
  printf("Creating channel\n");
  chid = ChannelCreate (0);
  printf("Channel created with id %d\n", chid);

  analogInputThread = initAnalogInputThread( chid, 0);
  analogOutputThread = initAnalogOutputThread( chid, 0);
  userInputThread = initUserInputThread( chid, 0);
  controlCalcThread = initControlCalculationThread( chid, 0);
}

void endThreads(){
  printf("Destroying channel with id %d\n", chid);
  ChannelDestroy(chid);
}
