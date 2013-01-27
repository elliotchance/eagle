#ifndef eagle_EagleLogger_h
#define eagle_EagleLogger_h

#include <stdio.h>
#include <mach/mach_time.h>
#include "EagleSynchronizer.h"
#include "EagleLinkedList.h"
#include "Eagle.h"
#include "EagleLoggerEvent.h"

typedef struct {
    
    /**
     The total messages that have been reported since this logged was initialised.
     */
    EAGLE_ATTR_NA int totalMessages;
    
    /**
     This lock synchronizes the addition of messages reported by multiple threads.
     */
    EAGLE_ATTR_MANAGED EagleLock *logLock;
    
    /**
     A file handle to output the messages to. This may be an actual file or stdout, stderr etc.
     */
    EAGLE_ATTR_PROVIDED FILE *out;
    
    /**
     The most recent event.
     */
    EAGLE_ATTR_MANAGED EagleLoggerEvent *lastEvent;
    
} EagleLogger;

/**
 Get the default logger. If the default logged has not been initialised it will be initialised automatically and
 returned.
 */
EagleLogger *EagleLogger_Get(void);

/**
 Log an event.
 */
EagleLoggerEvent* EagleLogger_Log(EagleLoggerSeverity severity, char *message);

void EagleLogger_LogEvent(EagleLoggerEvent *event);

/**
 Delete a logger.
 
 @note Do not call this function unless you have a good reason. Once the logger is created it will be reused
 automatically.
 
 @param [in] logger The logger to free.
 */
void EagleLogger_Delete(EagleLogger *logger);

/**
 Create a new logger. This is really only used for tests, you should use EagleLogger_Get() to get the default logger.
 @param [in] out The file stream to send the error messages to.
 @return A new logger.
 */
EagleLogger* EagleLogger_New(FILE *out);

EagleLoggerEvent* EagleLogger_log(EagleLogger* logger, EagleLoggerSeverity severity, char *message);

void EagleLogger_logEvent(EagleLogger* logger, EagleLoggerEvent *event);

EagleLoggerEvent* EagleLogger_LastEvent(void);

EagleLoggerEvent* EagleLogger_lastEvent(EagleLogger *logger);

#endif
