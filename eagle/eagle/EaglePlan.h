#ifndef eagle_EaglePlan_h
#define eagle_EaglePlan_h

#include <mach/mach_time.h>
#include "EaglePageOperations.h"
#include "EaglePlanOperation.h"
#include "EaglePlanBufferProvider.h"

/**
 Repressents the types of errors (or successes) that can occur inside a plan.
 */
typedef enum {
    
    /**
     Everything is fine.
     */
    EaglePlanErrorNone = 0,
    
    /**
     If an expression cannot be compiled (i.e. syntax error)
     */
    EaglePlanErrorCompile = 1,
    
    /**
     Unknown column / identifier.
     */
    EaglePlanErrorIdentifier = 2
    
} EaglePlanError;

typedef struct {
    
    /**
     The number of allocated operations.
     */
    int allocatedOperations;
    
    /**
     The number of used operations.
     */
    int usedOperations;
    
    /**
     The operations. An operation is a task done on a page.
     */
    EaglePlanOperation **operations;
    
    /**
     The number of allocated providers.
     */
    int allocatedProviders;
    
    /**
     The number of used providers.
     */
    int usedProviders;
    
    /**
     The page providers. This is where external data comes into the expression. For example, column data.
     */
    EaglePlanBufferProvider **providers;
    
    /**
     The amount of records to read per page.
     */
    int pageSize;
    
    /**
     The error status.
     */
    EaglePlanError errorCode;
    
    /**
     Further details about the error, this may be NULL.
     */
    char *errorMessage;
    
    /**
     Result set after execution.
     */
    EaglePageProvider **result;
    
    /**
     The number of providers in \c result
     */
    int resultFields;
    
    /**
     The total time the query took to execute.
     */
    uint64_t executionTime;
    
    /**
     Internal use for the timer.
     */
    uint64_t splitTime;
    
} EaglePlan;

EaglePlan* EaglePlan_New(int pageSize);
void EaglePlan_Delete(EaglePlan *plan);

void EaglePlan_addOperation(EaglePlan *plan, EaglePlanOperation *epo);
void EaglePlan_addBufferProvider(EaglePlan *plan, EaglePlanBufferProvider *bp);
EaglePage* EaglePlan_getBuffer(EaglePlan *plan, int buffer);
const char* EaglePlan_toString(EaglePlan *plan);
EaglePlanBufferProvider* EaglePlan_getBufferProviderByName(EaglePlan *plan, char *name);
void EaglePlan_setError(EaglePlan *plan, EaglePlanError errorCode, char *errorMessage);
EagleBoolean EaglePlan_isError(EaglePlan *plan);
void EaglePlan_resumeTimer(EaglePlan *plan);
void EaglePlan_stopTimer(EaglePlan *plan);
double EaglePlan_getExecutionSeconds(EaglePlan *plan);

#endif
