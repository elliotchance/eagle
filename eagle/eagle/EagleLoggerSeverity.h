#ifndef eagle_EagleLoggerSeverity_h
#define eagle_EagleLoggerSeverity_h

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

#endif
