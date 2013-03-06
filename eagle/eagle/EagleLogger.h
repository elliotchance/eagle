#ifndef eagle_EagleLogger_h
#define eagle_EagleLogger_h

#include <stdio.h>
#include <mach/mach_time.h>
#include "EagleSynchronizer.h"
#include "EagleLinkedList.h"
#include "Eagle.h"
#include "EagleLoggerEvent.h"

/**
 The location to save the logs.
 */
extern const char *EagleLogger_DefaultLogLocation;

/**
 A logger instance.
 */
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
 * Get the default logger. If the default logged has not been initialised it will be initialised automatically and
 * returned.
 * @return The default logger.
 */
EagleLogger* EagleLogger_Get(void);

/**
 * Log an event.
 * @param [in] severity The severity.
 * @param [in] message The message.
 * @return The event instance that was created.
 */
EagleLoggerEvent* EagleLogger_Log(EagleLoggerSeverity severity, char *message);

/**
 * Log an event with the default logger.
 * @param [in] event The event.
 */
void EagleLogger_LogEvent(EagleLoggerEvent *event);

/**
 * Delete a logger.
 * 
 * @note Do not call this function unless you have a good reason. Once the logger is created it will be reused
 * automatically.
 * 
 * @param [in] logger The logger to free.
 */
void EagleLogger_Delete(EagleLogger *logger);

/**
 * Create a new logger. This is really only used for tests, you should use EagleLogger_Get() to get the default logger.
 * @param [in] out The file stream to send the error messages to.
 * @return A new logger.
 */
EagleLogger* EagleLogger_New(FILE *out);

/**
 * Log an event.
 * @param [in] logger The logger.
 * @param [in] severity The severity.
 * @param [in] message The message.
 * @return The event instance that was created.
 */
EagleLoggerEvent* EagleLogger_log(EagleLogger* logger, EagleLoggerSeverity severity, char *message);

/**
 * Log an event.
 * @param [in] logger The logger,
 * @param [in] event The event.
 */
void EagleLogger_logEvent(EagleLogger* logger, EagleLoggerEvent *event);

/**
 * Get the last event from the default logger.
 * @return The last event or NULL if there have been no events.
 */
EagleLoggerEvent* EagleLogger_LastEvent(void);

/**
 * Get the last event from a logger.
 * @param [in] logger The logger.
 * @return The last event or NULL if there have been no events.
 */
EagleLoggerEvent* EagleLogger_lastEvent(EagleLogger *logger);

/**
 Get the file handle for a log location. If the log file cannot be open then stderr is returned.
 @param [in] location The file location.
 @return Always a valid FILE handle, even in case of failure.
 */
FILE* EagleLogger_GetLogFile(const char *location);

/**
 Reset a logger. Free the memory associated with the last error event.
 @param [in] logger The logger.
 */
void EagleLogger_reset(EagleLogger *logger);

#endif
