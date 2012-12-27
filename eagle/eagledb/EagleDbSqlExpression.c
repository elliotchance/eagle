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
 Return value when the expression can not be compiled.
 */
const int EagleDbSqlExpression_ERROR = -1;

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
int EagleDbSqlExpression_CompilePlanIntoBuffer_(EagleDbSqlExpression *expression, int *destinationBuffer, EaglePlan *plan)
{
    switch(expression->expressionType) {
        case EagleDbSqlExpressionTypeBinaryExpression:
        {
            EagleDbSqlBinaryExpression *cast = (EagleDbSqlBinaryExpression*) expression;
            int destination, destinationLeft, destinationRight;
            char *msg;
            EaglePlanOperation *epo;
            EaglePageOperationFunction(pageOperation);
            
            /* left */
            destinationLeft = EagleDbSqlExpression_CompilePlanIntoBuffer_(cast->left, destinationBuffer, plan);
            if(EagleTrue == EaglePlan_isError(plan)) {
                return EagleDbSqlExpression_ERROR;
            }
            
            /* right */
            destinationRight = EagleDbSqlExpression_CompilePlanIntoBuffer_(cast->right, destinationBuffer, plan);
            if(EagleTrue == EaglePlan_isError(plan)) {
                return EagleDbSqlExpression_ERROR;
            }
            
            /* operator */
            msg = (char*) malloc(256);
            destination = *destinationBuffer;
            
            switch(cast->op) {
                    
                case EagleDbSqlExpressionOperatorPlus:
                    sprintf(msg, "<%d> + <%d> -> <%d>", destinationLeft, destinationRight, destination);
                    pageOperation = EaglePageOperations_AdditionPage;
                    break;
                    
                case EagleDbSqlExpressionOperatorEquals:
                    sprintf(msg, "<%d> = <%d> -> <%d>", destinationLeft, destinationRight, destination);
                    pageOperation = EaglePageOperations_EqualsPage;
                    break;
                    
                case EagleDbSqlExpressionOperatorModulus:
                    sprintf(msg, "<%d> %% <%d> -> <%d>", destinationLeft, destinationRight, destination);
                    pageOperation = EaglePageOperations_ModulusPage;
                    break;
                
            }
            
            epo = EaglePlanOperation_New(pageOperation, destination, destinationLeft, destinationRight, NULL,
                                         EagleFalse, msg);
            free(msg);
            EaglePlan_addOperation(plan, epo);
            ++*destinationBuffer;
            
            return destination;
        }
            
        case EagleDbSqlExpressionTypeValue:
        {
            EagleDbSqlValue *value = (EagleDbSqlValue*) expression;
            
            switch(value->type) {
                case EagleDbSqlValueTypeInteger:
                {
                    int destination = *destinationBuffer;
                    EaglePageProvider *provider = EaglePageProvider_CreateFromInt(value->value.intValue, plan->pageSize, "(integer)");
                    EaglePlanBufferProvider *bp = EaglePlanBufferProvider_New(destination, provider, EagleTrue);
                    EaglePlan_addBufferProvider(plan, bp);
                    ++*destinationBuffer;
                    return destination;
                }
                    
                case EagleDbSqlValueTypeIdentifier:
                {
                    /* find the provider for this column */
                    EaglePlanBufferProvider *provider = EaglePlan_getBufferProviderByName(plan, value->value.identifier);
                    if(NULL == provider) {
                        char msg[128];
                        sprintf(msg, "Unknown column '%s'", value->value.identifier);
                        EaglePlan_setError(plan, EaglePlanErrorIdentifier, msg);
                        return EagleDbSqlExpression_ERROR;
                    }
                    
                    return provider->destinationBuffer;
                }
                    
                case EagleDbSqlValueTypeAsterisk:
                {
                    char msg[128];
                    sprintf(msg, "You can not use the star operator like this.");
                    EaglePlan_setError(plan, EaglePlanErrorCompile, msg);
                    return EagleDbSqlExpression_ERROR;
                }
            }
        }
            
        case EagleDbSqlExpressionTypeSelect:
            return 0;
    }
}

void EagleDbSqlExpression_CompilePlan(EagleDbSqlExpression **expressions, int totalExpressions, int whereClause, EaglePlan *plan)
{
    int i, *results;
    
    /* make sure we don't override buffers that are already assigned by providers */
    int destinationBuffer = 0;
    for(i = 0; i < plan->usedProviders; ++i) {
        if(plan->providers[i]->destinationBuffer >= destinationBuffer) {
            destinationBuffer = plan->providers[i]->destinationBuffer + 1;
        }
    }
    
    /* compile expressions */
    results = (int*) calloc((size_t) totalExpressions, sizeof(int));
    for(i = 0; i < totalExpressions; ++i) {
        results[i] = EagleDbSqlExpression_CompilePlanIntoBuffer_(expressions[i], &destinationBuffer, plan);
        
        if(EagleTrue == EaglePlan_isError(plan)) {
            free(results);
            return;
        }
    }
    
    /* if there is a WHERE clause expression only send those records */
    for(i = 0; i < totalExpressions; ++i) {
        EaglePlanOperation *epo;
        char msg[64];
        
        if(i != whereClause) {
            if(whereClause >= 0) {
                /* send some result data to the provider */
                sprintf(msg, "WHERE <%d>, send <%d> to provider %d", results[whereClause], results[i], i);
                epo = EaglePlanOperation_New(EaglePageOperations_SendIntPageToProvider, -1, results[whereClause], results[i], plan->result[i], EagleFalse, msg);
                EaglePlan_addOperation(plan, epo);
            }
            else {
                /* send all the result data to the provider */
                sprintf(msg, "ALL <%d> to provider %d", results[i], i);
                epo = EaglePlanOperation_New(EaglePageOperations_SendIntPageToProvider, -1, -1, results[i], plan->result[i], EagleFalse, msg);
                EaglePlan_addOperation(plan, epo);
            }
        }
    }
    
    free(results);
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

char* EagleDbSqlExpression_toString(EagleDbSqlExpression *expr)
{
    if(NULL == expr) {
        return "";
    }
    
    switch(expr->expressionType) {
        case EagleDbSqlExpressionTypeBinaryExpression:
            return EagleDbSqlBinaryExpression_toString((EagleDbSqlBinaryExpression*) expr);
            
        case EagleDbSqlExpressionTypeSelect:
            return EagleDbSqlSelect_toString((EagleDbSqlSelect*) expr);
            
        case EagleDbSqlExpressionTypeValue:
            return EagleDbSqlValue_toString((EagleDbSqlValue*) expr);
    }
}
