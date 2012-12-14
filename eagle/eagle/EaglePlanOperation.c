#include <stdlib.h>
#include <string.h>
#include "EaglePlanOperation.h"
#include "EagleData.h"

EaglePlanOperation* EaglePlanOperation_NewType(int type, int destination, EaglePageOperationFunction(function), int source, void *obj, const char *description)
{
    EaglePlanOperation *epo = (EaglePlanOperation*) malloc(sizeof(EaglePlanOperation));
    epo->type = type;
    epo->destination = destination;
    epo->source = source;
    epo->function = function;
    epo->obj = obj;
    epo->description = strdup(description);
    return epo;
}

EaglePlanOperation* EaglePlanOperation_New(int destination, EaglePageOperationFunction(function), int source, void *obj, const char *description)
{
    return EaglePlanOperation_NewType(EaglePlanOperation_NORMAL, destination, function, source, obj, description);
}

EaglePlanOperation* EaglePlanOperation_NewPage(int destination, EaglePageOperationFunction(function), int source1, int source2, const char *description)
{
    return EaglePlanOperation_NewType(EaglePlanOperation_PAGE, destination, function, source1, EagleData_Int(source2), description);
}

const char* EaglePlanOperation_toString(EaglePlanOperation *epo)
{
    return epo->description;
}

void EaglePlanOperation_Delete(EaglePlanOperation *epo)
{
    free(epo->obj);
    free((void*) epo->description);
    free(epo);
}
