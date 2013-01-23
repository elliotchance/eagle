#include "EagleLoggerSeverity.h"

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
