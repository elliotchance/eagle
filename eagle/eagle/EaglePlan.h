#ifndef eagle_EaglePlan_h
#define eagle_EaglePlan_h

#include "EaglePageOperations.h"
#include "EaglePlanOperation.h"
#include "EaglePageReceiver.h"
#include "EaglePlanBufferProvider.h"

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
     The receiver takes record IDs for which the expression in the plan evaluates to true.
     */
    EaglePageReceiver *receiver;
    
} EaglePlan;

EaglePlan* EaglePlan_New(int pageSize, EaglePageReceiver *receiver);
void EaglePlan_Delete(EaglePlan *plan);

void EaglePlan_addOperation(EaglePlan *plan, EaglePlanOperation *epo);
void EaglePlan_addBufferProvider(EaglePlan *plan, EaglePlanBufferProvider *bp);
EaglePage* EaglePlan_getBuffer(EaglePlan *plan, int buffer);
const char* EaglePlan_toString(EaglePlan *plan);
EaglePlanBufferProvider* EaglePlan_getBufferProviderByName(EaglePlan *plan, char *name);

#endif
