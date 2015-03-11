/*
 * AnalogInputThread.h
 *
 *  Created on: Mar 11, 2015
 *      Author: Brian
 */

#ifndef ANALOGINPUTTHREAD_H_
#define ANALOGINPUTTHREAD_H_

typedef struct {
  int ch_id;
  int thread_id;
  //TODO define additional args
} AnalogInputThreadArgs;

pthread_t *initAnalogInputThread(int chid, int threadId); //TODO define args


#endif /* ANALOGINPUTTHREAD_H_ */
