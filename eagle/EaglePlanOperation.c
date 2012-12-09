#include <stdlib.h>
#include "EaglePlanOperation.h"

EaglePlanOperation* EaglePlanOperation_New(int destination, EaglePageOperationFunction(function), int source, void *obj, const char *description)
{
    EaglePlanOperation *epo = (EaglePlanOperation*) malloc(sizeof(EaglePlanOperation));
    epo->destination = destination;
    epo->source = source;
    epo->function = function;
    epo->obj = obj;
    epo->description = description;
    return epo;
}

const char* EaglePlanOperation_toString(EaglePlanOperation *epo)
{
    return epo->description;
}
