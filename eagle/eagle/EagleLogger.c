#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "EagleLogger.h"
#include "EagleSynchronizer.h"

static EagleLogger *EagleLogger_Logger = NULL;

EagleLoggerEvent* EagleLoggerEvent_New(EagleLoggerSeverity severity, char *message)
{
    EagleLoggerEvent *event = (EagleLoggerEvent*) malloc(sizeof(EagleLoggerEvent));
    
    event->when = time(NULL);
    event->severity = severity;
    event->message = strdup(message);
    event->obj = NULL;
    
    return event;
}

void EagleLoggerEvent_Delete(EagleLoggerEvent *event)
{
    free(event->message);
    free(event);
}

EagleLogger *EagleLogger_Get(void)
{
    /* this function must be synchronized */
    EagleLock *lock = EagleSynchronizer_CreateLock();
    EagleSynchronizer_Lock(lock);
    
    /* initialise if its not started */
    if(NULL == EagleLogger_Logger) {
        EagleLogger_Logger = (EagleLogger*) malloc(sizeof(EagleLogger_Logger));
        
        EagleLogger_Logger->totalMessages = 0;
        EagleLogger_Logger->logLock = EagleSynchronizer_CreateLock();
        EagleLogger_Logger->out = stderr;
    }
    
    EagleSynchronizer_Unlock(lock);
    return EagleLogger_Logger;
}

EagleLoggerEvent* EagleLogger_Log(EagleLoggerSeverity severity, char *message)
{
    EagleLoggerEvent *event = EagleLoggerEvent_New(severity, message);
    EagleLogger_LogEvent(event);
    return event;
}

void EagleLogger_LogEvent(EagleLoggerEvent *event)
{
    EagleLogger *logger = EagleLogger_Get();
    char *timestamp;
    
    /* this function must be synchronized */
    EagleSynchronizer_Lock(logger->logLock);
    
    /* pretty time */
    timestamp = ctime(&event->when);
    timestamp[strlen(timestamp) - 1] = '\0';

    fprintf(logger->out, "%s: [%s] %s\n", timestamp, EagleLoggerSeverity_toString(event->severity), event->message);
    
    EagleSynchronizer_Unlock(logger->logLock);
}

char* EagleLoggerSeverity_toString(EagleLoggerSeverity severity)
{
    switch(severity) {
            
        case EagleLoggerSeverityDebug:
            return "DEBUG";
            
        case EagleLoggerSeverityInfo:
            return "INFO";
            
        case EagleLoggerSeverityUserError:
            return "USERERROR";
            
        case EagleLoggerSeverityError:
            return "ERROR";
            
        case EagleLoggerSeverityFatal:
            return "FATAL";
            
    }
}
