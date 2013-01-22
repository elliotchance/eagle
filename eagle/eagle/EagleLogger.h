#ifndef eagle_EagleLogger_h
#define eagle_EagleLogger_h

#include <mach/mach_time.h>
#include <stdio.h>
#include "EagleSynchronizer.h"
#include "EagleLinkedList.h"

/**
 Every logged event has a severity. This is a value between 0 (the least severe) to 100 (the most severe).
 */
typedef enum {
    
    /**
     A fatal error means that the program can not continue. This is the most serious level.
     */
    EagleLoggerSeverityFatal = 100,
    
    /**
     An error is a problem that has stopped a task from continuing but the state of the program is recoverable and it
     will continue.
     */
    EagleLoggerSeverityError = 50,
    
    /**
     This is an error because it stopped the execution of a task, but the error is not within eagle. A bad input has
     been provided. A good example would be a mistake in the SQL syntax of a query.
     */
    EagleLoggerSeverityUserError = 25,
    
    /**
     This describes events in the system that have no effect on the flow of execution, such as start up and shutdown
     messages.
     */
    EagleLoggerSeverityInfo  = 10,
    
    /**
     Debug messages can be very verbose and explain this that are happening that may have nothing to do with the actual
     execution.
     */
    EagleLoggerSeverityDebug = 1
    
} EagleLoggerSeverity;

char* EagleLoggerSeverity_toString(EagleLoggerSeverity severity);

/**
 A single logged event.
 */
typedef struct {
    
    /**
     When the event was raised.
     */
    time_t when;
    
    /**
     How severe the event was.
     */
    EagleLoggerSeverity severity;
    
    /**
     A message describing the event.
     */
    char *message;
    
    /**
     An optional attached object.
     */
    void *obj;
    
} EagleLoggerEvent;

EagleLoggerEvent* EagleLoggerEvent_New(EagleLoggerSeverity severity, char *message);
void EagleLoggerEvent_Delete(EagleLoggerEvent *event);

typedef struct {
    
    int totalMessages;
    
    EagleLock *logLock;
    
    FILE *out;
    
} EagleLogger;

EagleLogger *EagleLogger_Get(void);

EagleLoggerEvent* EagleLogger_Log(EagleLoggerSeverity severity, char *message);

void EagleLogger_LogEvent(EagleLoggerEvent *event);

/**
 Delete a logger.
 
 @note Do not call this function unless you have a good reason. Once the logger is created it will be reused
 automatically.
 
 @param [in] logger The logger to free.
 */
void EagleLogger_Delete(EagleLogger *logger);

#endif
