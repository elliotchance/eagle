#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "EagleLogger.h"
#include "EagleSynchronizer.h"
#include "EagleMemory.h"

static EagleLogger *EagleLogger_Logger = NULL;

void EagleLogger_Delete(EagleLogger *logger)
{
    if(NULL == logger) {
        return;
    }
    
    EagleLock_Delete(logger->logLock);
    EagleMemory_Free(logger);
}

EagleLogger *EagleLogger_Get(void)
{
    /* this function must be synchronized */
    EagleLock *lock = EagleSynchronizer_CreateLock();
    EagleSynchronizer_Lock(lock);
    
    /* initialise if its not started */
    if(NULL == EagleLogger_Logger) {
        EagleLogger_Logger = (EagleLogger*) EagleMemory_Allocate("EagleLogger_Get.1", sizeof(EagleLogger_Logger));
        if(NULL == EagleLogger_Logger) {
            EagleLock_Delete(lock);
            return NULL;
        }
        
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
    
    if(NULL == logger) {
        return;
    }
    if(NULL == event) {
        return;
    }
    
    /* this function must be synchronized */
    EagleSynchronizer_Lock(logger->logLock);
    
    /* pretty time */
    timestamp = ctime(&event->when);
    timestamp[strlen(timestamp) - 1] = '\0';

    fprintf(logger->out, "%s: [%s] %s\n", timestamp, EagleLoggerSeverity_toString(event->severity), event->message);
    
    EagleSynchronizer_Unlock(logger->logLock);
}
