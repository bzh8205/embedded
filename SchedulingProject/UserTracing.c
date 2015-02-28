#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/trace.h>
#include "UserTracing.h"


int eventId;
const char * _programName;

//Don't know if this is actually needed.
void initUserTracing( const char * programName ) {
  eventId = 0;
  _programName = programName;
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

void userTraceEvent( int event_id, unsigned int info){
  trace_logi( event_id, info, eventId );
  eventId++;
  //TRACE_EVENT( _programName, TraceEvent(_NTO_TRACE_INSERTUSRSTREVENT, event_id, str ));
}
