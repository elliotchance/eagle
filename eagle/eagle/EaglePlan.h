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
    
    EaglePlanErrorNone = 0,       /**< Everything is fine. */
    EaglePlanErrorCompile = 1,    /**< If an expression cannot be compiled (i.e. syntax error) */
    EaglePlanErrorIdentifier = 2, /**< Unknown column / identifier. */
    EaglePlanErrorNoSuchTable = 4 /**< The table name does not exist in the schema. */
    
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
    
    /**
     This is the total amount of time the plan has spent waiting for locks.
     */
    uint64_t lockWaitTime;
    
    /**
     The number of buffers needed for the execution.
     */
    int buffersNeeded;
    
    /**
     The types for each buffer.
     */
    EagleDataType *bufferTypes;
    
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
void EaglePlan_prepareBuffers(EaglePlan *plan, int buffers);
int EaglePlan_getRealResultFields(EaglePlan *plan);

#endif
