#ifndef eagle_EagleLoggerEvent_h
#define eagle_EagleLoggerEvent_h

#include <time.h>
#include "EagleLoggerSeverity.h"
#include "Eagle.h"

/**
 A single logged event.
 */
typedef struct {
    
    /**
     When the event was raised.
     */
    EAGLE_ATTR_NA time_t when;
    
    /**
     How severe the event was.
     */
    EAGLE_ATTR_NA EagleLoggerSeverity severity;
    
    /**
     A message describing the event.
     */
    EAGLE_ATTR_MANAGED char *message;
    
    /**
     An optional attached object.
     */
    EAGLE_ATTR_PROVIDED void *obj;
    
} EagleLoggerEvent;

/**
 Create a new logger event.
 @param [in] severity How severe the event is.
 @param [in] message The message.
 @return A new logger event instance.
 */
EagleLoggerEvent* EagleLoggerEvent_New(EagleLoggerSeverity severity, char *message);

/**
 Delete a logger event.
 
 @note If you have sent the event to a logger do not use this function, the logger will take care of freeing the event
       when it no longer needs it.
 
 @param [in] event The event to release.
 */
void EagleLoggerEvent_Delete(EagleLoggerEvent *event);

#endif
