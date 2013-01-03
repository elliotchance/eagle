#include <stdlib.h>
#include <string.h>
#include "EaglePlanOperation.h"
#include "EagleData.h"
#include "EagleMemory.h"

EaglePlanOperation* EaglePlanOperation_New(EaglePageOperationFunction(function), int destination, int source1, int source2, void *obj, EagleBoolean freeObj, const char *description)
{
    EaglePlanOperation *epo = (EaglePlanOperation*) malloc(sizeof(EaglePlanOperation));
    
    epo->function = function;
    epo->destination = destination;
    epo->source1 = source1;
    epo->source2 = source2;
    epo->obj = obj;
    epo->freeObj = freeObj;
    epo->description = strdup(description);
    
    return epo;
}

char* EaglePlanOperation_toString(EaglePlanOperation *epo)
{
    return strdup(epo->description);
}

void EaglePlanOperation_Delete(EaglePlanOperation *epo)
{
    if(EagleTrue == epo->freeObj) {
        EagleMemory_Free(epo->obj);
    }
    EagleMemory_Free((void*) epo->description);
    EagleMemory_Free(epo);
}
