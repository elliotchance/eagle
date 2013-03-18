#include <stdlib.h>
#include <string.h>
#include "EaglePlanOperation.h"
#include "EagleData.h"
#include "EagleMemory.h"

EaglePlanOperation* EaglePlanOperation_NewWithPage(EaglePageOperationFunction(function),
                                                   int destination,
                                                   int source1,
                                                   int source2,
                                                   void *obj,
                                                   EagleBoolean freeObj,
                                                   const char *description)
{
    EaglePlanOperation *epo = (EaglePlanOperation*) EagleMemory_Allocate("EaglePlanOperation_NewWithPage.1", sizeof(EaglePlanOperation));
    if(NULL == epo) {
        return NULL;
    }
    
    epo->type = EaglePlanOperationTypePage;
    epo->function = function;
    epo->destination = destination;
    epo->source1 = source1;
    epo->source2 = source2;
    epo->obj = obj;
    epo->freeObj = freeObj;
    epo->description = (NULL == description ? NULL : strdup(description));
    
    return epo;
}

EaglePlanOperation* EaglePlanOperation_NewWithLiteral(EaglePageOperationFunction(function),
                                                      int destination,
                                                      int source1,
                                                      EagleDbSqlValue *literal,
                                                      EagleBoolean freeLiteral,
                                                      const char *description)
{
    EaglePlanOperation *epo = (EaglePlanOperation*) EagleMemory_Allocate("EaglePlanOperation_NewWithLiteral.1", sizeof(EaglePlanOperation));
    if(NULL == epo) {
        return NULL;
    }
    
    epo->type = EaglePlanOperationTypeLiteral;
    epo->function = function;
    epo->destination = destination;
    epo->source1 = source1;
    epo->obj = literal;
    epo->freeObj = freeLiteral;
    epo->description = (NULL == description ? NULL : strdup(description));
    
    return epo;
}

char* EaglePlanOperation_toString(EaglePlanOperation *epo)
{
    return strdup(epo->description);
}

void EaglePlanOperation_Delete(EaglePlanOperation *epo)
{
    if(NULL == epo) {
        return;
    }
    
    if(EagleTrue == epo->freeObj) {
        EagleMemory_Free(epo->obj);
    }
    EagleMemory_Free((void*) epo->description);
    EagleMemory_Free(epo);
}
