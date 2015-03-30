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
  ANALOG_IN_START, //EVENT 5
  ANALOG_IN_END, //EVENT 6
  CALC_START, //EVENT 7
  CALC_END, //EVENT 8
  ANALOG_OUT_START, //EVENT 9
  ANALOG_OUT_END //EVENT 10
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

void logEvent(EVENT_TYPE et, int info);

#endif /* LOGGINGUTILITY_H_ */
