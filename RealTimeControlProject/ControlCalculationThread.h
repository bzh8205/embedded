/*
 * ControlCalculationThread.h
 *
 *  Created on: Mar 11, 2015
 *      Author: Brian
 */

#ifndef CONTROLCALCULATIONTHREAD_H_
#define CONTROLCALCULATIONTHREAD_H_



typedef struct {
  int ch_id;
  int thread_id;
  //TODO define additional args
} ControlCalculationThreadArgs;

pthread_t *initControlCalculationThread(int chid, int threadId); //TODO define args
void setPIDConstants( float target, float Kp, float Ki, float Kd );
long getTimeUs();
#ifndef A_Diff_EQ 
float calculatePIDOutput(float pidInput);
#else
double calculatePIDOutput(double measuredPoint, double setPoint);
#endif

#endif /* CONTROLCALCULATIONTHREAD_H_ */
