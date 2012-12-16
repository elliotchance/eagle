#include <stdlib.h>
#include <stdio.h>
#include "EagleDbSqlExpression.h"
#include "EagleDbSqlValue.h"
#include "EaglePageProvider.h"
#include "EagleData.h"
#include "EagleDbSqlBinaryExpression.h"
#include "EaglePageOperations.h"
#include "EagleDbSqlSelect.h"

/**
 Recursive function to compile an expression into a plan.
 
 @param expression The expression to compile.
 @param destinationBuffer This number is incremented throughout the recursion. When you first call this function the
        \p destinationBuffer will be the position of the first buffer allowed to be writen to. You will likely want to
        set this to 1 so that you keep buffer 0 available. Note that this is not where there final result will be put,
        it is only the start ID of which buffers can be written to - the function will return an int which is the buffer
        ID with the real result. You may need to copy that buffer into buffer 0.
 @return The buffer ID that contains the real result.
 */
int EagleDbSqlExpression_CompilePlanIntoBuffer_(EagleDbSqlExpression *expression, int destinationBuffer, EaglePlan *plan)
{
    int finalDestination = destinationBuffer;
    
    switch(expression->expressionType) {
        case EagleDbSqlExpressionTypeBinaryExpression:
        {
            EagleDbSqlBinaryExpression *cast = (EagleDbSqlBinaryExpression*) expression;
            int destinationLeft, destinationRight;
            char *msg;
            EaglePlanOperation *epo;
            
            /* left */
            destinationLeft = destinationBuffer;
            EagleDbSqlExpression_CompilePlanIntoBuffer_(cast->left, destinationBuffer, plan);
            ++destinationBuffer;
            
            /* right */
            destinationRight = destinationBuffer;
            EagleDbSqlExpression_CompilePlanIntoBuffer_(cast->right, destinationBuffer, plan);
            ++destinationBuffer;
            
            /* operator */
            msg = (char*) malloc(256);
            sprintf(msg, "dest = %d, source1 = %d, source2 = %d", destinationBuffer, destinationLeft, destinationRight);
            epo = EaglePlanOperation_New(EaglePageOperations_AdditionPage, destinationBuffer, destinationLeft,
                                         destinationRight, NULL, EagleFalse, msg);
            free(msg);
            EaglePlan_addOperation(plan, epo);
            
            finalDestination = destinationBuffer;
            break;
        }
            
        case EagleDbSqlExpressionTypeValue:
        {
            EagleDbSqlValue *value = (EagleDbSqlValue*) expression;
            EaglePageProvider *provider = EaglePageProvider_CreateFromInt(value->value.intValue, plan->pageSize);
            EaglePlanBufferProvider *bp = EaglePlanBufferProvider_New(destinationBuffer, provider);
            EaglePlan_addBufferProvider(plan, bp);
            
            finalDestination = destinationBuffer;
            break;
        }
            
        case EagleDbSqlExpressionTypeSelect:
            break;
    }
    
    return finalDestination;
}

void EagleDbSqlExpression_CompilePlanIntoProvider(EagleDbSqlExpression *expression, EaglePageProvider *destination, EaglePlan *plan)
{
    int result;
    EaglePlanOperation *epo;
    
    /* evaluate the expression as usual */
    result = EagleDbSqlExpression_CompilePlanIntoBuffer_(expression, 0, plan);
    
    /* send all the result data to the provider */
    epo = EaglePlanOperation_New(EaglePageOperations_SendIntPageToProvider, -1, result, -1, destination, EagleFalse, "Send to provider");
}

/**
 Compile an expression into a plan.
 
 @param expression The expression to compile.
 @param destinationBuffer Which buffer should the boolean result go into?
 @param plan This must be an initialised EaglePlan, execution steps will be appended onto this.
 */
void EagleDbSqlExpression_CompilePlanIntoBoolean(EagleDbSqlExpression *expression, int destinationBuffer, EaglePlan *plan)
{
    int finalDestination = EagleDbSqlExpression_CompilePlanIntoBuffer_(expression, destinationBuffer, plan);
    
    /* cast the final result into a boolean */
    if(0 != finalDestination) {
        char *msg = (char*) malloc(256);
        EaglePlanOperation *epo;
        
        sprintf(msg, "CastIntToBool: dest = %d, source1 = %d", 0, finalDestination);
        epo = EaglePlanOperation_New(EaglePageOperations_CastIntPageToBoolean, 0, finalDestination, -1, NULL, EagleFalse, msg);
        free(msg);
        EaglePlan_addOperation(plan, epo);
    }
}

void EagleDbSqlExpression_Delete(EagleDbSqlExpression *expr)
{
    if(NULL == expr) {
        return;
    }
    
    switch(expr->expressionType) {
        case EagleDbSqlExpressionTypeBinaryExpression:
            EagleDbSqlBinaryExpression_Delete((EagleDbSqlBinaryExpression*) expr);
            break;
            
        case EagleDbSqlExpressionTypeSelect:
            EagleDbSqlSelect_Delete((EagleDbSqlSelect*) expr);
            break;
            
        case EagleDbSqlExpressionTypeValue:
            EagleDbSqlValue_Delete((EagleDbSqlValue*) expr);
            break;
    }
}
