#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "EagleLogger.h"
#include "EagleSynchronizer.h"
#include "EagleMemory.h"
#include "EagleWorker.h"

const char *EagleLogger_DefaultLogLocation = "/tmp/eagledb.log";

void EagleLogger_Delete(EagleLogger *logger)
{
    if(NULL == logger) {
        return;
    }
    
    if(NULL != logger->out && stderr != logger->out && stdout != logger->out) {
        fclose(logger->out);
        logger->out = NULL;
    }
    
    EagleLock_Delete(logger->logLock);
    EagleLoggerEvent_Delete(logger->lastEvent);
    EagleMemory_Free(logger);
}

void EagleLogger_reset(EagleLogger *logger)
{
    EagleLoggerEvent_Delete(logger->lastEvent);
    logger->lastEvent = NULL;
}

EagleLogger* EagleLogger_New(FILE *out)
{
    EagleLogger *logger = (EagleLogger*) EagleMemory_Allocate("EagleLogger_New.1", 1000);
    if(NULL == logger) {
        return NULL;
    }
    
    logger->totalMessages = 0;
    logger->logLock = EagleSynchronizer_CreateLock();
    logger->out = out;
    logger->lastEvent = NULL;
    
    return logger;
}

EagleLoggerEvent* EagleLogger_LastEvent()
{
    return EagleLogger_lastEvent(EagleLogger_Get());
}

EagleLoggerEvent* EagleLogger_lastEvent(EagleLogger *logger)
{
    if(NULL == logger) {
        return NULL;
    }
    return logger->lastEvent;
}

FILE* EagleLogger_GetLogFile(const char *location)
{
    FILE *logFile = fopen(location, "a");
    if(NULL == logFile) {
        /*fprintf(stderr, "Unable to open log file for writing '%s'.", location);*/
        logFile = stderr;
    }
    
    return logFile;
}

EagleLogger* EagleLogger_Get(void)
{
    static EagleLogger *EagleLogger_Logger = NULL;
    static EagleLock *lock = NULL;
    
    /* this function must be synchronized */
    if(NULL == lock) {
        lock = EagleSynchronizer_CreateLock();
    }
    EagleSynchronizer_Lock(lock);
    
    /* initialise if its not started */
    if(NULL == EagleLogger_Logger) {
        /* use the default log location */
        FILE *logFile = EagleLogger_GetLogFile(EagleLogger_DefaultLogLocation);
        
        EagleLogger_Logger = EagleLogger_New(logFile);
    }
    
    EagleSynchronizer_Unlock(lock);
    return EagleLogger_Logger;
}

EagleLoggerEvent* EagleLogger_log(EagleLogger* logger, EagleLoggerSeverity severity, char *message)
{
    EagleLoggerEvent *event = EagleLoggerEvent_New(severity, message);
    EagleLogger_logEvent(logger, event);
    return event;
}

EagleLoggerEvent* EagleLogger_Log(EagleLoggerSeverity severity, char *message)
{
    return EagleLogger_log(EagleLogger_Get(), severity, message);
}

void EagleLogger_logEvent(EagleLogger* logger, EagleLoggerEvent *event)
{
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
    
    if(NULL != logger->out) {
        fprintf(logger->out, "%s: [%s] %s\n", timestamp, EagleLoggerSeverity_toString(event->severity), event->message);
        fflush(logger->out);
    }
    ++logger->totalMessages;
    
    /* track most recent event */
    if(NULL != logger->lastEvent) {
        EagleLoggerEvent_Delete(logger->lastEvent);
    }
    logger->lastEvent = event;
    
    EagleSynchronizer_Unlock(logger->logLock);
}

void EagleLogger_LogEvent(EagleLoggerEvent *event)
{
    EagleLogger_logEvent(EagleLogger_Get(), event);
}
