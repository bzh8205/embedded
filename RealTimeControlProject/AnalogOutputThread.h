/*
 * AnalogOutputThread.h
 *
 *  Created on: Mar 11, 2015
 *      Author: Brian
 */

#ifndef ANALOGOUTPUTTHREAD_H_
#define ANALOGOUTPUTTHREAD_H_

typedef struct {
  int ch_id;
  int thread_id;
  //TODO define additional args
} AnalogOutputThreadArgs;

pthread_t *initAnalogOutputThread(int chid, int threadId); //TODO define args


#endif /* ANALOGOUTPUTTHREAD_H_ */
