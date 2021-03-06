/*
 * LoggingUtility.c
 *
 *  Created on: Mar 30, 2015
 *      Author: Brian
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/trace.h>
#include "LoggingUtility.h"

//#define CONSOLE_PRINT

int eventId;
const char * _programName;
const char *eventNames[11];

//Don't know if this is actually needed.
void initUserTracing( const char * programName ) {
  eventId = 0;
  _programName = programName;
#ifdef CONSOLE_PRINT
  eventNames[0] = "RUN_START";
  eventNames[1] = "RUN_END";
  eventNames[2] = "KP_SET";
  eventNames[3] = "KI_SET";
  eventNames[4] = "KD_SET";
  eventNames[5] = "PULSE_RECV";
  eventNames[6] = "ANALOG_IN_RESULT";
  eventNames[7] = "CALC_RESULT";
  eventNames[8] = "ANALOG_OUT_END";
  eventNames[9] = "PID_ERROR";
  eventNames[10] = "SETPOINT_SET";
#endif
  /*
   * Just in case, turn off all filters, since we
   * don't know their present state - go to the
   * known state of the filters.
   */
  TRACE_EVENT(_programName, TraceEvent(_NTO_TRACE_DELALLCLASSES));
  TRACE_EVENT(_programName, TraceEvent(_NTO_TRACE_CLRCLASSPID, _NTO_TRACE_KERCALL));
  TRACE_EVENT(_programName, TraceEvent(_NTO_TRACE_CLRCLASSTID, _NTO_TRACE_KERCALL));
  TRACE_EVENT(_programName, TraceEvent(_NTO_TRACE_CLRCLASSPID, _NTO_TRACE_THREAD));
  TRACE_EVENT(_programName, TraceEvent(_NTO_TRACE_CLRCLASSTID, _NTO_TRACE_THREAD));

  /*
   * Set fast emitting mode for all classes and
   * their events.
   */
  TRACE_EVENT(_programName, TraceEvent(_NTO_TRACE_SETALLCLASSESFAST));

  /*
   * Intercept all event classes
   */
  TRACE_EVENT(_programName, TraceEvent(_NTO_TRACE_ADDALLCLASSES));
  TRACE_EVENT(_programName, TraceEvent(_NTO_TRACE_START));
}

void logEvent(EVENT_TYPE et, float info) {
  trace_logf((int)et,"UserEvent: %f num: %d", info, eventId);
#ifdef CONSOLE_PRINT
  printf("%s(event #%d): %f\n",  eventNames[(int)et], eventId, info);
#endif
  eventId++;
}
