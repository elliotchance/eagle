#include <string.h>
#include "EagleLoggerEvent.h"
#include "EagleMemory.h"

EagleLoggerEvent* EagleLoggerEvent_New(EagleLoggerSeverity severity, char *message)
{
    EagleLoggerEvent *event = (EagleLoggerEvent*) EagleMemory_Allocate("EagleLoggerEvent_New.1", sizeof(EagleLoggerEvent));
    if(NULL == event) {
        return NULL;
    }
    
    event->when = time(NULL);
    event->severity = severity;
    event->message = (NULL == message ? NULL : strdup(message));
    event->obj = NULL;
    
    return event;
}

void EagleLoggerEvent_Delete(EagleLoggerEvent *event)
{
    if(NULL == event) {
        return;
    }
    
    EagleMemory_Free(event->message);
    EagleMemory_Free(event);
}
