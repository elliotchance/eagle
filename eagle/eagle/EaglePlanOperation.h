#ifndef eagle_EaglePlanOperation_h
#define eagle_EaglePlanOperation_h

#include "EaglePageOperations.h"
#include "EaglePageProvider.h"
#include "EagleBoolean.h"
#include "Eagle.h"

/**
 Represents a plan operation, or a single step in the executation of an expression.
 */
typedef struct {
    
    /**
     Destination buffer ID.
     */
    EAGLE_ATTR_NA int destination;
    
    /**
     First argument buffer ID.
     */
    EAGLE_ATTR_NA int source1;
    
    /**
     Second buffer argument ID.
     */
    EAGLE_ATTR_NA int source2;
    
    /**
     An optional attachment object.
     */
    EAGLE_ATTR_SEMI_MANAGED void *obj;
    
    /**
     If EagleTrue the \c obj will be freed when the EaglePlanOperation is freed.
     */
    EAGLE_ATTR_NA EagleBoolean freeObj;
    
    /**
     The function pointer to perform the operation.
     */
    EAGLE_ATTR_NA EaglePageOperationFunction(function);
    
    /**
     A description of this operation.
     */
    EAGLE_ATTR_MANAGED const char *description;
    
} EaglePlanOperation;

EaglePlanOperation* EaglePlanOperation_New(EaglePageOperationFunction(function), int destination, int source1, int source2, void *obj, EagleBoolean freeObj, const char *description);

void EaglePlanOperation_Delete(EaglePlanOperation *epo);

char* EaglePlanOperation_toString(EaglePlanOperation *epo);

#endif
