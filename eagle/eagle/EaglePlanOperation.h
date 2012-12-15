#ifndef eagle_EaglePlanOperation_h
#define eagle_EaglePlanOperation_h

#include "EaglePageOperations.h"
#include "EaglePageProvider.h"
#include "EagleBoolean.h"

#define EaglePlanOperation_NORMAL 1
#define EaglePlanOperation_PAGE   2

typedef struct {
    int destination;
    int source;
    EaglePageOperationFunction(function);
    void *obj;
    EagleBoolean freeObj;
    const char *description;
    int type;
} EaglePlanOperation;

EaglePlanOperation* EaglePlanOperation_New(int destination, EaglePageOperationFunction(function), int source, void *obj, EagleBoolean freeObj, const char *description);
EaglePlanOperation* EaglePlanOperation_NewPage(int destination, EaglePageOperationFunction(function), int source1, int source2, const char *description);
EaglePlanOperation* EaglePlanOperation_NewType(int type, int destination, EaglePageOperationFunction(function), int source, void *obj, EagleBoolean freeObj, const char *description);
void EaglePlanOperation_Delete(EaglePlanOperation *epo);
const char* EaglePlanOperation_toString(EaglePlanOperation *epo);

#endif
