#ifndef eagle_EaglePlan_h
#define eagle_EaglePlan_h

#include <mach/mach_time.h>
#include "EaglePageOperations.h"
#include "EaglePlanOperation.h"
#include "EaglePlanBufferProvider.h"
#include "EaglePlanError.h"
#include "Eagle.h"
#include "EagleDataType.h"

/**
 An execution plan.
 */
typedef struct {
    
    /**
     The number of allocated operations.
     */
    EAGLE_ATTR_NA int allocatedOperations;
    
    /**
     The number of used operations.
     */
    EAGLE_ATTR_NA int usedOperations;
    
    /**
     The operations. An operation is a task done on a page. This is semi managed because the actual array that contains
     the operations will be managed by the object, but the individual operations will be managed externally.
     */
    EAGLE_ATTR_SEMI_MANAGED EaglePlanOperation **operations;
    
    /**
     The number of allocated providers.
     */
    EAGLE_ATTR_NA int allocatedProviders;
    
    /**
     The number of used providers.
     */
    EAGLE_ATTR_NA int usedProviders;
    
    /**
     The page providers. This is where external data comes into the expression.
     */
    EAGLE_ATTR_MANAGED EaglePlanBufferProvider **providers;
    
    /**
     The amount of records to read per page.
     */
    EAGLE_ATTR_NA int pageSize;
    
    /**
     The error status.
     */
    EAGLE_ATTR_NA EaglePlanError errorCode;
    
    /**
     Further details about the error, this may be NULL.
     */
    EAGLE_ATTR_MANAGED char *errorMessage;
    
    /**
     Result set after execution.
     */
    EAGLE_ATTR_MANAGED EaglePageProvider **result;
    
    /**
     The number of providers in \c result
     */
    EAGLE_ATTR_NA int resultFields;
    
    /**
     The total time the query took to execute.
     */
    EAGLE_ATTR_NA uint64_t executionTime;
    
    /**
     Internal use for the timer.
     */
    EAGLE_ATTR_NA uint64_t splitTime;
    
    /**
     This is the total amount of time the plan has spent waiting for locks.
     */
    EAGLE_ATTR_NA uint64_t lockWaitTime;
    
    /**
     The number of buffers needed for the execution.
     */
    EAGLE_ATTR_NA int buffersNeeded;
    
    /**
     The types for each buffer.
     */
    EAGLE_ATTR_MANAGED EagleDataType *bufferTypes;
    
    /**
     This is a stack of objects to be freed when this object is deleted.
     */
    EAGLE_ATTR_MANAGED EagleLinkedList *freeObjects;
    
} EaglePlan;

EaglePlan* EaglePlan_New(int pageSize);

void EaglePlan_Delete(EaglePlan *plan);

void EaglePlan_addOperation(EaglePlan *plan, EaglePlanOperation *epo);

void EaglePlan_addBufferProvider(EaglePlan *plan, EaglePlanBufferProvider *bp, EagleBoolean free);

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

void EaglePlan_addFreeObject(EaglePlan *plan, void *obj, void (*free)(void*));

#endif
