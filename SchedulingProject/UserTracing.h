#ifndef USERTRACING_H_
#define USERTRACING_H_

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

/*
 * \brief inserts a new user event
 */
void userTraceEvent( int event_id, unsigned int info);




#endif /* Scheduler.h */
