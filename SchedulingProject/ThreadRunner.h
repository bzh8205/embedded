/*
 * ThreadRunner.h
 *
 *  Created on: Feb 27, 2015
 *      Author: Brian
 */

#ifndef THREADRUNNER_H_
#define THREADRUNNER_H_

#include "Workload.h"

void TaskThread(void *arguments);
pthread_t *initTaskThread(int chid, int threadId, int execTimeUs);
void initThreadRunner(Workload *workload);
void runThread(int num);
void destroyThreadRunner();

#endif /* THREADRUNNER_H_ */
