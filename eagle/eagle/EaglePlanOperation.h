#ifndef eagle_EaglePlanOperation_h
#define eagle_EaglePlanOperation_h

#include "EaglePageOperations.h"
#include "EaglePageProvider.h"
#include "EagleBoolean.h"
#include "Eagle.h"
#include "EagleDbSqlValue.h"

/**
 The plan operation type.
 */
typedef enum {
    
    EaglePlanOperationTypePage = 1,    /**< Page operation. */
    
    EaglePlanOperationTypeLiteral = 2  /**< Literal operation. */
    
} EaglePlanOperationType;

/**
 Represents a plan operation, or a single step in the executation of an expression.
 */
typedef struct {
    
    /**
     The plan operation type.
     */
    EAGLE_ATTR_NA EaglePlanOperationType type;
    
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

/**
 * Create a new plan operation with a page operation.
 * 
 * @param [in] destination Buffer ID for the destination, this can be less than zero for no buffer.
 * @param [in] source1 Buffer ID for the left operand, this can be less than zero for no buffer.
 * @param [in] source2 Buffer ID for the right operand, this can be less than zero for no buffer.
 * @param [in] obj An attached object, some page operations need this.
 * @param [in] freeObj Free the \p obj when deleting the EaglePlanOperation.
 * @param [in] description A human readable description to be rendered into EaglePlanOperation_toString()
 * @return A new EaglePlanOperation.
 */
EaglePlanOperation* EaglePlanOperation_NewWithPage(EaglePageOperationFunction(function),
                                                   int destination,
                                                   int source1,
                                                   int source2,
                                                   void *obj,
                                                   EagleBoolean freeObj,
                                                   const char *description);

/**
 * Create a new plan operation with a literal.
 *
 * @param [in] destination Buffer ID for the destination, this can be less than zero for no buffer.
 * @param [in] source1 Buffer ID for the left operand, this can be less than zero for no buffer.
 * @param [in] literal The value.
 * @param [in] freeLiteral Free the \p literal when deleting the EaglePlanOperation.
 * @param [in] description A human readable description to be rendered into EaglePlanOperation_toString()
 * @return A new EaglePlanOperation.
 */
EaglePlanOperation* EaglePlanOperation_NewWithLiteral(EaglePageOperationFunction(function),
                                                      int destination,
                                                      int source1,
                                                      EagleDbSqlValue *literal,
                                                      EagleBoolean freeLiteral,
                                                      const char *description);

/**
 * Delete an operation.
 * @param [in] epo The instance.
 */
void EaglePlanOperation_Delete(EaglePlanOperation *epo);

/**
 * Get the string representation of this operation. This is actually just a copy of the description that was passed in
 * EaglePlanOperation_New(), you must free the description when your finished with it.
 * @param [in] epo The instance.
 * @return A string copy of the description.
 */
char* EaglePlanOperation_toString(EaglePlanOperation *epo);

#endif
