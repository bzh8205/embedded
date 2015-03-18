/*
 * UserInputThread.h
 *
 *  Created on: Mar 11, 2015
 *      Author: Brian
 */

#ifndef USERINPUTTHREAD_H_
#define USERINPUTTHREAD_H_

// 0 is init, 1 is run, -1 is exit
int USER_IN_RUN_THREADS;

typedef struct {
  int ch_id;
  int thread_id;
  //TODO define additional args
} UserInputThreadArgs;

pthread_t *initUserInputThread(int chid, int threadId); //TODO define args
void getPIDInput( float *target, float *Kp, float *Ki, float *Kd);

#endif /* USERINPUTTHREAD_H_ */
