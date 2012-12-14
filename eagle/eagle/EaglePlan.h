#ifndef eagle_EaglePlan_h
#define eagle_EaglePlan_h

#include "EaglePageOperations.h"
#include "EaglePlanOperation.h"
#include "EaglePageReceiver.h"
#include "EaglePlanBufferProvider.h"

typedef struct {
    int allocatedOperations;
    int usedOperations;
    EaglePlanOperation **operations;
    int allocatedProviders;
    int usedProviders;
    EaglePlanBufferProvider **providers;
    int pageSize;
    EaglePageReceiver *receiver;
} EaglePlan;

EaglePlan* EaglePlan_New(int pageSize, EaglePageReceiver *receiver);
void EaglePlan_Delete(EaglePlan *plan);

void EaglePlan_addOperation(EaglePlan *plan, EaglePlanOperation *epo);
void EaglePlan_addBufferProvider(EaglePlan *plan, EaglePlanBufferProvider *bp);
EaglePage* EaglePlan_getBuffer(EaglePlan *plan, int buffer);
const char* EaglePlan_toString(EaglePlan *plan);

#endif
