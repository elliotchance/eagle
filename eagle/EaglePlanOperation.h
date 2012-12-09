#ifndef eagle_EaglePlanOperation_h
#define eagle_EaglePlanOperation_h

#include "EaglePageOperations.h"
#include "EaglePageProvider.h"

typedef struct {
    int destination;
    int source;
    EaglePageOperationFunction(function);
    EaglePageReceiver *pageReceiver;
    void *obj;
    const char *description;
} EaglePlanOperation;

EaglePlanOperation* EaglePlanOperation_New(int destination, EaglePageOperationFunction(function), int source, void *obj, const char *description);
const char* EaglePlanOperation_toString(EaglePlanOperation *epo);

#endif
