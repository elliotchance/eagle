#ifndef eagle_EaglePlanOperation_h
#define eagle_EaglePlanOperation_h

#include "EaglePageOperations.h"
#include "EaglePageProvider.h"
#include "EagleBoolean.h"

/**
 Represents a plan operation, or a single step in the executation of an expression.
 */
typedef struct {
    
    /**
     Destination buffer ID.
     */
    int destination;
    
    /**
     First argument buffer ID.
     */
    int source1;
    
    /**
     Second buffer argument ID.
     */
    int source2;
    
    /**
     An optional attachment object.
     */
    void *obj;
    
    /**
     If EagleTrue the \c obj will be freed when the EaglePlanOperation is freed.
     */
    EagleBoolean freeObj;
    
    /**
     The function pointer to perform the operation.
     */
    EaglePageOperationFunction(function);
    
    /**
     A description of this operation.
     */
    const char *description;
    
} EaglePlanOperation;

EaglePlanOperation* EaglePlanOperation_New(EaglePageOperationFunction(function), int destination, int source1, int source2, void *obj, EagleBoolean freeObj, const char *description);
void EaglePlanOperation_Delete(EaglePlanOperation *epo);
char* EaglePlanOperation_toString(EaglePlanOperation *epo);

#endif
