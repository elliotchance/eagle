#ifndef eagle_EaglePlan_h
#define eagle_EaglePlan_h

#include <mach/mach_time.h>
#include "EaglePageOperations.h"
#include "EaglePlanOperation.h"
#include "EaglePlanBufferProvider.h"
#include "EaglePlanError.h"
#include "Eagle.h"
#include "EagleDataType.h"
#include "EagleLinkedList.h"

/**
 An execution plan.
 */
typedef struct {
    
    /**
     The operations. An operation is a task done on a page. This is semi managed because the actual array that contains
     the operations will be managed by the object, but the individual operations will be managed externally.
     */
    EAGLE_ATTR_SEMI_MANAGED EagleLinkedList *operations;
    
    /**
     The page providers. This is where external data comes into the expression.
     */
    EAGLE_ATTR_MANAGED EagleLinkedList *providers;
    
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
    EAGLE_ATTR_MANAGED uint64_t *executionTime;
    
    /**
     The total time the thread was left waiting for a mutex lock to be release.
     */
    EAGLE_ATTR_MANAGED uint64_t *lockTime;
    
    /**
     The absolute start time.
     */
    EAGLE_ATTR_NA uint64_t startTime;
    
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
    
    /**
     The number of CPU cores.
     */
    EAGLE_ATTR_NA int cores;
    
} EaglePlan;

/**
 * Create a new plan.
 * @param [in] pageSize The default page size for the providers.
 * @param [in] cores The number of CPU cores.
 * @return A new plan instance.
 */
EaglePlan* EaglePlan_New(int pageSize, int cores);

/**
 * Delete a plan.
 * @param [in] plan The plan.
 */
void EaglePlan_Delete(EaglePlan *plan);

/**
 * Add an operation to a plan.
 * @param [in] plan The plan.
 * @param [in] epo The operation.
 */
void EaglePlan_addOperation(EaglePlan *plan, EaglePlanOperation *epo);

/**
 * Add a buffer provider to a plan.
 * @param [in] plan The plan.
 * @param [in] bp The buffer provider.
 * @param [in] free Free the buffer provider with the plan?
 */
void EaglePlan_addBufferProvider(EaglePlan *plan, EaglePlanBufferProvider *bp, EagleBoolean free);

/**
 * Get a buffer by its index.
 * @param [in] plan The plan.
 * @param [in] buffer The buffer ID.
 * @return The eagle page for the buffer.
 */
EaglePage* EaglePlan_getBuffer(EaglePlan *plan, int buffer);

/**
 * Render the plan as a descriptive string.
 * @param [in] plan The plan.
 * @return A new string, you must free it when you are finished with it.
 */
const char* EaglePlan_toString(EaglePlan *plan);

/**
 * Get a buffer provider by name.
 * @param [in] plan The plan.
 * @param [in] name The name of the buffer provider.
 * @return NULL if it cannot be found, otherwise the buffer provider will be returned.
 */
EaglePlanBufferProvider* EaglePlan_getBufferProviderByName(EaglePlan *plan, char *name);

/**
 * Set the error for the plan.
 * @param [in] plan The plan.
 * @param [in] errorCode The error code.
 * @param [in] errorMessage The message describing the error.
 */
void EaglePlan_setError(EaglePlan *plan, EaglePlanError errorCode, char *errorMessage);

/**
 * Check if the plan is currently in an error state.
 * @param [in] plan The plan.
 * @return boolean value.
 */
EagleBoolean EaglePlan_isError(EaglePlan *plan);

/**
 * Get the total execution time for all CPUs in seconds. This does not include IO wait time, only CPU time.
 * @param [in] plan The plan.
 * @return Number of CPU seconds.
 */
double EaglePlan_getExecutionSeconds(EaglePlan *plan);

/**
 * Get the total wait time for all CPUs in seconds.
 * @param [in] plan The plan.
 * @return Number of CPU seconds.
 */
double EaglePlan_getLockSeconds(EaglePlan *plan);

/**
 * Prepare the buffers before the expression can be compiled.
 * @param [in] plan The plan.
 * @param [in] buffers The amount of buffers you will need to carry out all of the expressions.
 */
void EaglePlan_prepareBuffers(EaglePlan *plan, int buffers);

/**
 * The real result fields is the number of visible columns. It does not included computer columns like the WHERE clause.
 * @param [in] plan The plan.
 * @return The number of visible result fields.
 */
int EaglePlan_getRealResultFields(EaglePlan *plan);

/**
 * Extra objects to be freed when this plan instance is removed.
 * @param [in] plan The plan.
 * @param [in] obj The attached object.
 * @param [in] free The Delete function required to free it. You may use NULL for EagleMemory_Free()
 */
void EaglePlan_addFreeObject(EaglePlan *plan, void *obj, void (*free)(void*));

/**
 * Get the real execution seconds. This is the absolute time difference from when the plan begins executing until now.
 * @param [in] plan The plan.
 * @return The number of absolute execution seconds.
 */
double EaglePlan_getRealExecutionSeconds(EaglePlan *plan);

#endif
