/*
 * LoggingUtility.h
 *
 *  Created on: Mar 30, 2015
 *      Author: Brian
 */

#ifndef LOGGINGUTILITY_H_
#define LOGGINGUTILITY_H_


typedef enum {
  RUN_START, // EVENT 0
  RUN_END, //EVENT 1
  KP_SET, //EVENT 2
  KI_SET, //EVENT 3
  KD_SET, //EVENT 4
  PULSE_RECV, //EVENT 5
  ANALOG_IN_RESULT, //EVENT 6
  CALC_RESULT, //EVENT 7
  ANALOG_OUT_END, //EVENT 8
  PID_ERROR, //EVENT 9
  TARGET_SET //EVENT 10
/*  ANALOG_IN_START, //EVENT 5
  ANALOG_IN_END, //EVENT 6
  CALC_START, //EVENT 7
  CALC_END, //EVENT 8
  ANALOG_OUT_START, //EVENT 9 */
} EVENT_TYPE;

#define TRACE_EVENT(prog_name, trace_event) \
if((int)((trace_event))==(-1)) \
{ \
    (void) fprintf \
    ( \
     stderr, \
     "%s: line:%d function call TraceEvent() failed, errno(%d): %s\n", \
     prog_name, \
     __LINE__, \
     errno, \
     strerror(errno) \
    ); \
 \
}

/*
 * \brief inits usertracing with the programs id
 */
void initUserTracing( const char * programName);

void logEvent(EVENT_TYPE et, float info);

#endif /* LOGGINGUTILITY_H_ */
