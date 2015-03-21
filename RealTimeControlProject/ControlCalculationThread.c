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

#ifndef A_Diff_EQ
static float TARGET = 0;
static float K_P = 0;
static float K_I = 0;
static float K_D = 0;
static float prevError = 0;
static float integral = 0;
static float error = 0;
static float derivative = 0;
static float dt = 0;

float calculatePIDOutput(float pidInput){
  float result;
  dt = (float)getTimeUs();
  error = TARGET - pidInput;
  integral = integral + error * dt;
  if( dt != 0 ) {
    derivative = (error - prevError)/dt;
  }
  result = K_P*error + K_I*integral + K_D*derivative;
  prevError = error;
  return result;
}
#else
//from Analog input thread
double SP;      //TODO set point,ANALOG input as AI1
double MP;      // measured input, ANALOG input as AI0 (from ADC/DAC unit)
//history for PID
double u_kp1=0;   //storing current measurement calculated
double u_k=0;     //previous measurement
double e_kp1=0;   //current error
double e_k=0;     // previous error
double e_km1=0;   //error measurement before the previous
//from user input thread
static float K_P = 0;
static float K_I = 0;
static float K_D = 0;

/**
 * \brief Calculates the next output
 * \brief from a provided road input, and the previous road input
 * the wheel (and car) output is calulated to feed into the plant simulator 
 */
//TODO provide SP and MP as params from Analog input thread
float calculatePIDOutput(float errorInput) {
    double wheel;
    e_kp1 = SP-MP;    // calculate error for this current instance
    //calculate next step
    u_kp1 = u_k + (K_P+K_I+K_D)*e_k_p1+ (K_P+2*K_D)*e_k+(K_D)*e_km1;
    //store output
    wheel = u_kp1;
    //update variables for next calculation    
    u_k = u_kp1;    //current output will be last 
    e_km1 = e_k;    //error 2 calls ago will be from 1 call ago
    e_k = e_kp1;    //current error will be last

    return wheel; 
}

#endif

//util variables
unsigned char is_time_init;
struct timespec init_time;

void ControlCalculationThread(void *arguments) {
  int rcvid;
  ThreadMessage message;
  ControlCalculationThreadArgs *args = (ControlCalculationThreadArgs *) arguments;
  int chid = args->ch_id;
  int threadId = args->thread_id;
  printf("ControlCalculationThread %d created\n", threadId);

  //run until message signals to exit
  while( 1 ){
    rcvid = MsgReceive(chid, &message, sizeof(message), NULL);
    if( message.exit != 1 ){
      //update z variables... this might not be a thing...
      message.value = calculatePIDOutput(message.value);
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



long getTimeUs(){
  struct timespec current;
  long result, seconds, microseconds;
  if( is_time_init == 0 ){
    if( clock_gettime( CLOCK_REALTIME, &init_time ) == -1 ){
      perror("clock_gettime");
    }
    is_time_init = 1;
    result = 0L;
  } else {
    if( clock_gettime( CLOCK_REALTIME, &current ) == -1 ){
      perror("clock_gettime");
    }

    seconds = (long)current.tv_sec - init_time.tv_sec;
    microseconds = (long)((current.tv_nsec - init_time.tv_nsec)/1000);
    result = seconds*1000000 + microseconds;

    //result = (long)( ((current.tv_sec - init_time.tv_sec) * 1000) +
    //    ((current.tv_nsec - init_time.tv_nsec)/100000) );
  }
  return result;
}

void setPIDConstants( float target, float Kp, float Ki, float Kd ){
  TARGET = target;
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
