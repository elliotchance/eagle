#include <stdlib.h>
#include <stdio.h>
#include "EagleDbSqlExpression.h"
#include "EagleDbSqlValue.h"
#include "EaglePageProvider.h"
#include "EagleData.h"
#include "EagleDbSqlBinaryExpression.h"
#include "EaglePageOperations.h"

int EagleDbSqlExpression_CompilePlan(EagleDbSqlExpression *expression, int destinationBuffer, EaglePlan *plan, int depth)
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
            EagleDbSqlExpression_CompilePlan(cast->left, destinationBuffer, plan, depth + 1);
            ++destinationBuffer;
            
            /* right */
            destinationRight = destinationBuffer;
            EagleDbSqlExpression_CompilePlan(cast->right, destinationBuffer, plan, depth + 1);
            ++destinationBuffer;
            
            /* operator */
            msg = (char*) malloc(256);
            sprintf(msg, "dest = %d, source1 = %d, source2 = %d", destinationBuffer, destinationLeft, destinationRight);
            epo = EaglePlanOperation_NewPage(destinationBuffer, EaglePageOperations_AdditionPage, destinationLeft, destinationRight, msg);
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
    
    /* cast the final result into a boolean */
    if(0 != finalDestination && 0 == depth) {
        char *msg = (char*) malloc(256);
        EaglePlanOperation *epo;
        
        sprintf(msg, "CastIntToBool: dest = %d, source1 = %d", 0, finalDestination);
        epo = EaglePlanOperation_NewPage(0, EaglePageOperations_CastIntPageToBoolean, finalDestination, 0, msg);
        EaglePlan_addOperation(plan, epo);
    }
    
    return finalDestination;
}
