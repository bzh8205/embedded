/*
 * UserInputThread.h
 *
 *  Created on: Mar 11, 2015
 *      Author: Brian
 */

#ifndef USERINPUTTHREAD_H_
#define USERINPUTTHREAD_H_

typedef struct {
  int ch_id;
  int thread_id;
  //TODO define additional args
} UserInputThreadArgs;

pthread_t *initUserInputThread(int chid, int threadId); //TODO define args

#endif /* USERINPUTTHREAD_H_ */
