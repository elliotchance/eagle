#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EagleDbSqlExpression.h"
#include "EagleDbSqlValue.h"
#include "EaglePageProvider.h"
#include "EagleData.h"
#include "EagleDbSqlBinaryExpression.h"
#include "EagleDbSqlUnaryExpression.h"
#include "EaglePageOperations.h"
#include "EagleDbSqlSelect.h"
#include "EagleDbSqlBinaryExpressionOperator.h"
#include "EagleDbSqlUnaryExpressionOperator.h"
#include "EagleMemory.h"
#include "EaglePageProviderSingle.h"
#include "EaglePageProviderStream.h"
#include "EagleDbSqlFunctionExpression.h"
#include "EagleUtils.h"
#include "EagleDbSqlCastExpression.h"

const int EagleDbSqlExpression_ERROR = -1;

int EagleDbSqlExpression_CompilePlanIntoBuffer_Function_(EagleDbSqlExpression *expression,
                                                         int *destinationBuffer,
                                                         EaglePlan *plan)
{
    EagleDbSqlFunctionExpression *cast = (EagleDbSqlFunctionExpression*) expression;
    int destination, destinationExpr;
    EaglePlanOperation *epo;
    EaglePageOperationFunction(pageOperation);
    char msg[1024], *t1, *t2;
    
    /* expression */
    destinationExpr = EagleDbSqlExpression_CompilePlanIntoBuffer_(cast->expr, destinationBuffer, plan, EagleFalse);
    if(EagleDbSqlExpression_ERROR == destinationExpr || EagleTrue == EaglePlan_isError(plan)) {
        return EagleDbSqlExpression_ERROR;
    }
    
    destination = *destinationBuffer;
    
    /* find the function */
    if(EagleUtils_CompareWithoutCase("sqrt", cast->name) && plan->bufferTypes[destinationExpr] == EagleDataTypeFloat) {
        pageOperation = EaglePageOperations_SqrtPageFloat;
    }
    else {
        /* function does not exist */
        t1 = EagleDataType_typeToName(plan->bufferTypes[destinationExpr]);
        sprintf(msg, "Function %s(%s) does not exist.", cast->name, t1);
        EagleMemory_Free(t1);
        
        EaglePlan_setError(plan, EaglePlanErrorIdentifier, msg);
        return EagleDbSqlExpression_ERROR;
    }
    
    plan->bufferTypes[destination] = EagleDataTypeFloat;
    
    t1 = EagleDataType_typeToName(plan->bufferTypes[destinationExpr]);
    t2 = EagleDataType_typeToName(plan->bufferTypes[destination]);
    sprintf(msg, "{ %s(<%d>) (%s) } into <%d> (%s)", cast->name, destinationExpr, t1, destination, t2);
    EagleMemory_Free(t1);
    EagleMemory_Free(t2);
    
    epo = EaglePlanOperation_NewWithPage(pageOperation, destination, destinationExpr, -1, NULL, EagleFalse, msg);
    EaglePlan_addOperation(plan, epo);
    EaglePlan_addFreeObject(plan, epo, (void(*)(void*)) EaglePlanOperation_Delete);
    ++*destinationBuffer;
    
    return destination;
}

int EagleDbSqlExpression_CompilePlanIntoBuffer_Unary_(EagleDbSqlExpression *expression,
                                                      int *destinationBuffer,
                                                      EaglePlan *plan)
{
    EagleDbSqlUnaryExpression *cast = (EagleDbSqlUnaryExpression*) expression;
    int destination, destinationLeft;
    char msg[1024], *t1, *t2, *beforeOp = NULL, *afterOp = NULL;
    EaglePlanOperation *epo;
    EagleDbSqlUnaryOperator matchOp;
    EagleBoolean matchedOp;
    
    /* operand */
    destinationLeft = EagleDbSqlExpression_CompilePlanIntoBuffer_(cast->expr, destinationBuffer, plan, EagleFalse);
    if(EagleDbSqlExpression_ERROR == destinationLeft || EagleTrue == EaglePlan_isError(plan)) {
        return EagleDbSqlExpression_ERROR;
    }
    
    /* catch grouping operator */
    if(EagleDbSqlUnaryExpressionOperatorGrouping == cast->op) {
        return destinationLeft;
    }
    
    /* operator */
    destination = *destinationBuffer;
    
    t1 = EagleDataType_typeToName(plan->bufferTypes[destinationLeft]);
    EagleDbSqlUnaryExpressionOperator_toString(cast->op, &beforeOp, &afterOp);
    
    matchedOp = EagleDbSqlUnaryExpression_GetOperation(cast->op,
                                                       plan->bufferTypes[destinationLeft],
                                                       &matchOp);
    if(EagleFalse == matchedOp) {
        /* operator does not exist */
        sprintf(msg, "No such unary operator %s%s%s", beforeOp, t1, afterOp);
        EaglePlan_setError(plan, EaglePlanErrorIdentifier, msg);
        
        EagleMemory_Free(t1);
        EagleMemory_Free(beforeOp);
        EagleMemory_Free(afterOp);
        return EagleDbSqlExpression_ERROR;
    }
    
    plan->bufferTypes[destination] = EagleDataTypeInteger;
    
    t2 = EagleDataType_typeToName(plan->bufferTypes[destination]);
    
    sprintf(msg, "{ %s<%d>%s (%s) } into <%d> (%s)", beforeOp, destinationLeft, afterOp, t2, destination, t2);
    
    EagleMemory_Free(t1);
    EagleMemory_Free(t2);
    EagleMemory_Free(beforeOp);
    EagleMemory_Free(afterOp);
    
    epo = EaglePlanOperation_NewWithPage(matchOp.func, destination, destinationLeft, -1, NULL, EagleFalse, msg);
    EaglePlan_addOperation(plan, epo);
    EaglePlan_addFreeObject(plan, epo, (void(*)(void*)) EaglePlanOperation_Delete);
    ++*destinationBuffer;
    
    return destination;
}

int EagleDbSqlExpression_CompilePlanIntoBuffer_Cast_(const EagleDbSqlExpression *expression,
                                                     int *destinationBuffer,
                                                     EaglePlan *plan)
{
    EagleDbSqlCastExpression *cast = (EagleDbSqlCastExpression*) expression;
    int destination, destinationLeft;
    char msg[1024], *t1, *t2, *t3;
    EaglePlanOperation *epo;
    EagleDbSqlCastOperator matchOp;
    EagleBoolean matchedOp;
    
    /* left */
    destinationLeft = EagleDbSqlExpression_CompilePlanIntoBuffer_(cast->expr, destinationBuffer, plan, EagleFalse);
    if(EagleDbSqlExpression_ERROR == destinationLeft || EagleTrue == EaglePlan_isError(plan)) {
        return EagleDbSqlExpression_ERROR;
    }
    
    /* operator */
    destination = *destinationBuffer;
    
    t1 = EagleDataType_typeToName(plan->bufferTypes[destinationLeft]);
    t2 = EagleDataType_typeToName(cast->castAs);
    
    matchedOp = EagleDbSqlCastExpression_GetOperation(plan->bufferTypes[destinationLeft],
                                                      cast->castAs,
                                                      &matchOp);
    if(EagleFalse == matchedOp) {
        /* operator does not exist */
        sprintf(msg, "Can not cast %s to %s.", t1, t2);
        EaglePlan_setError(plan, EaglePlanErrorIdentifier, msg);
        
        EagleMemory_Free(t1);
        EagleMemory_Free(t2);
        return EagleDbSqlExpression_ERROR;
    }
    
    plan->bufferTypes[destination] = matchOp.right;
    
    t3 = EagleDataType_typeToName(plan->bufferTypes[destination]);
    
    sprintf(msg, "{ CAST <%d> (%s) } into <%d> (%s)", destinationLeft, t1, destination, t3);
    
    EagleMemory_Free(t1);
    EagleMemory_Free(t2);
    EagleMemory_Free(t3);
    
    epo = EaglePlanOperation_NewWithPage(matchOp.func, destination, destinationLeft, -1, NULL, EagleFalse, msg);
    EaglePlan_addOperation(plan, epo);
    EaglePlan_addFreeObject(plan, epo, (void(*)(void*)) EaglePlanOperation_Delete);
    ++*destinationBuffer;
    
    return destination;
}

EagleBoolean EagleDbSqlExpression_isLiteral(const EagleDbSqlExpression *expression)
{
    EagleBoolean r = EagleFalse;
    
    switch(expression->expressionType) {
            
        case EagleDbSqlExpressionTypeValue:
        {
            EagleDbSqlValue *value = (EagleDbSqlValue*) expression;
            switch(value->type) {
                    
                case EagleDbSqlValueTypeAsterisk:
                case EagleDbSqlValueTypeIdentifier:
                    break;
                    
                case EagleDbSqlValueTypeFloat:
                case EagleDbSqlValueTypeInteger:
                case EagleDbSqlValueTypeString:
                    r = EagleTrue;
                    break;
                    
            }
        }
            
        case EagleDbSqlExpressionTypeBinaryExpression:
        case EagleDbSqlExpressionTypeCastExpression:
        case EagleDbSqlExpressionTypeFunctionExpression:
        case EagleDbSqlExpressionTypeSelect:
        case EagleDbSqlExpressionTypeUnaryExpression:
            break;
            
    }
    
    return r;
}

int EagleDbSqlExpression_CompilePlanIntoBuffer_Binary_(const EagleDbSqlExpression *expression,
                                                       int *destinationBuffer,
                                                       EaglePlan *plan)
{
    EagleDbSqlBinaryExpression *cast = (EagleDbSqlBinaryExpression*) expression;
    int destination, destinationLeft, destinationRight;
    char msg[1024], *t1, *t2, *t3, *op;
    EaglePlanOperation *epo;
    EagleDbSqlBinaryOperator matchOp;
    EagleBoolean matchedOp, leftLiteral, rightLiteral;
    
    /* left */
    destinationLeft = EagleDbSqlExpression_CompilePlanIntoBuffer_(cast->left, destinationBuffer, plan, EagleFalse);
    if(EagleDbSqlExpression_ERROR == destinationLeft || EagleTrue == EaglePlan_isError(plan)) {
        return EagleDbSqlExpression_ERROR;
    }
    
    /* right */
    destinationRight = EagleDbSqlExpression_CompilePlanIntoBuffer_(cast->right, destinationBuffer, plan, EagleFalse);
    if(EagleDbSqlExpression_ERROR == destinationRight || EagleTrue == EaglePlan_isError(plan)) {
        return EagleDbSqlExpression_ERROR;
    }
    
    /* decide what type of binary expression it is */
    leftLiteral = EagleDbSqlExpression_isLiteral(cast->left);
    rightLiteral = EagleDbSqlExpression_isLiteral(cast->right);
    
    if(EagleTrue == leftLiteral && EagleTrue == rightLiteral) {
        /* this is a bit stupid and inefficient, but to handle a binary operation on two literals we create providers
         for each literal and handle it like a page operation. Issue #98 will address this. */
        
        /* notice that we do not check the error status after these because the error status has already been checked
         above */
        destinationLeft = EagleDbSqlExpression_CompilePlanIntoBuffer_(cast->left, destinationBuffer, plan, EagleTrue);
        destinationRight = EagleDbSqlExpression_CompilePlanIntoBuffer_(cast->right, destinationBuffer, plan, EagleTrue);
    }
    
    if(EagleFalse == leftLiteral && EagleTrue == rightLiteral) {
        matchedOp = EagleDbSqlBinaryExpression_GetLeftOperation(plan->bufferTypes[destinationLeft],
                                                                cast->op,
                                                                &matchOp);
    }
    else if(EagleTrue == leftLiteral && EagleFalse == rightLiteral) {
        matchedOp = EagleDbSqlBinaryExpression_GetRightOperation(plan->bufferTypes[destinationRight],
                                                                 cast->op,
                                                                 &matchOp);
    }
    else {
        matchedOp = EagleDbSqlBinaryExpression_GetPageOperation(plan->bufferTypes[destinationLeft],
                                                                cast->op,
                                                                plan->bufferTypes[destinationRight],
                                                                &matchOp);
    }
    
    /* operator */
    destination = *destinationBuffer;
    
    t1 = EagleDataType_typeToName(plan->bufferTypes[destinationLeft]);
    t2 = EagleDataType_typeToName(plan->bufferTypes[destinationRight]);
    op = EagleDbSqlBinaryExpressionOperator_toString(cast->op);
    
    if(EagleFalse == matchedOp) {
        /* operator does not exist */
        sprintf(msg, "No such binary operator %s %s %s", t1, op, t2);
        EaglePlan_setError(plan, EaglePlanErrorIdentifier, msg);
        
        EagleMemory_Free(t1);
        EagleMemory_Free(t2);
        EagleMemory_Free(op);
        return EagleDbSqlExpression_ERROR;
    }
    
    plan->bufferTypes[destination] = matchOp.returnType;
    
    t3 = EagleDataType_typeToName(plan->bufferTypes[destination]);
    
    if(EagleFalse == leftLiteral && EagleTrue == rightLiteral) {
        sprintf(msg, "{ <%d> (%s) %s %s } into <%d> (%s)", destinationLeft, t1, op, t2, destination, t3);
        epo = EaglePlanOperation_NewWithLiteral(matchOp.func, destination, destinationLeft, (EagleDbSqlValue*) cast->right, EagleFalse, msg);
    }
    else if(EagleTrue == leftLiteral && EagleFalse == rightLiteral) {
        sprintf(msg, "{ <%d> (%s) %s %s } into <%d> (%s)", destinationRight, t2, op, t1, destination, t3);
        epo = EaglePlanOperation_NewWithLiteral(matchOp.func, destination, destinationRight, (EagleDbSqlValue*) cast->left, EagleFalse, msg);
    }
    else {
        sprintf(msg, "{ <%d> (%s) %s <%d> (%s) } into <%d> (%s)", destinationLeft, t1, op, destinationRight, t2,
                destination, t3);
        epo = EaglePlanOperation_NewWithPage(matchOp.func, destination, destinationLeft, destinationRight, NULL, EagleFalse, msg);
    }
    
    EagleMemory_Free(t1);
    EagleMemory_Free(t2);
    EagleMemory_Free(t3);
    EagleMemory_Free(op);
    
    EaglePlan_addOperation(plan, epo);
    EaglePlan_addFreeObject(plan, epo, (void(*)(void*)) EaglePlanOperation_Delete);
    ++*destinationBuffer;
    
    return destination;
}

int EagleDbSqlExpression_CompilePlanIntoBuffer_Value_(EagleDbSqlExpression *expression,
                                                      int *destinationBuffer,
                                                      EaglePlan *plan,
                                                      EagleBoolean useProvider)
{
    EagleDbSqlValue *value = (EagleDbSqlValue*) expression;
    int destination;
    
    switch(value->type) {
            
        case EagleDbSqlValueTypeInteger:
        {
            if(EagleTrue == useProvider) {
                EaglePageProvider *provider;
                EaglePlanBufferProvider *bp;
                
                destination = *destinationBuffer;
                provider = (EaglePageProvider*) EaglePageProviderSingle_NewInt(value->value.intValue, plan->pageSize, "(integer literal)");
                bp = EaglePlanBufferProvider_NewWithProvider(destination, provider, EagleTrue);
                EaglePlan_addBufferProvider(plan, bp, EagleTrue);
                ++*destinationBuffer;
            }
            else {
                destination = *destinationBuffer;
            }
            
            plan->bufferTypes[destination] = EagleDataTypeInteger;
            break;
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
            
            plan->bufferTypes[provider->destinationBuffer] = provider->value.provider.provider->type;
            destination = provider->destinationBuffer;
            break;
        }
            
        case EagleDbSqlValueTypeAsterisk:
        {
            char msg[128];
            sprintf(msg, "You can not use the star operator like this.");
            EaglePlan_setError(plan, EaglePlanErrorCompile, msg);
            destination = EagleDbSqlExpression_ERROR;
            break;
        }
            
        case EagleDbSqlValueTypeString:
        {
            if(EagleTrue == useProvider) {
                EaglePageProvider *provider;
                EaglePlanBufferProvider *bp;
                
                destination = *destinationBuffer;
                provider = (EaglePageProvider*) EaglePageProviderSingle_NewVarchar(value->value.identifier, plan->pageSize, "(string literal)");
                bp = EaglePlanBufferProvider_NewWithProvider(destination, provider, EagleTrue);
                EaglePlan_addBufferProvider(plan, bp, EagleTrue);
                ++*destinationBuffer;
            }
            else {
                destination = *destinationBuffer;
            }
            
            plan->bufferTypes[destination] = EagleDataTypeVarchar;
            break;
        }
            
        case EagleDbSqlValueTypeFloat:
        {
            if(EagleTrue == useProvider) {
                EaglePageProvider *provider;
                EaglePlanBufferProvider *bp;
                
                destination = *destinationBuffer;
                provider = (EaglePageProvider*) EaglePageProviderSingle_NewFloat(value->value.floatValue, plan->pageSize, "(float)");
                bp = EaglePlanBufferProvider_NewWithProvider(destination, provider, EagleTrue);
                EaglePlan_addBufferProvider(plan, bp, EagleTrue);
                ++*destinationBuffer;
            }
            else {
                destination = *destinationBuffer;
            }
            
            plan->bufferTypes[destination] = EagleDataTypeFloat;
            break;
        }
            
    }
    
    return destination;
}

int EagleDbSqlExpression_CompilePlanIntoBuffer_(EagleDbSqlExpression *expression,
                                                int *destinationBuffer,
                                                EaglePlan *plan,
                                                EagleBoolean useProviderForValue)
{
    if(NULL == expression) {
        return EagleDbSqlExpression_ERROR;
    }
    if(NULL == plan) {
        return EagleDbSqlExpression_ERROR;
    }
    
    switch(expression->expressionType) {
            
        case EagleDbSqlExpressionTypeFunctionExpression:
            return EagleDbSqlExpression_CompilePlanIntoBuffer_Function_(expression, destinationBuffer, plan);
            
        case EagleDbSqlExpressionTypeUnaryExpression:
            return EagleDbSqlExpression_CompilePlanIntoBuffer_Unary_(expression, destinationBuffer, plan);
            
        case EagleDbSqlExpressionTypeBinaryExpression:
            return EagleDbSqlExpression_CompilePlanIntoBuffer_Binary_(expression, destinationBuffer, plan);
            
        case EagleDbSqlExpressionTypeValue:
            return EagleDbSqlExpression_CompilePlanIntoBuffer_Value_(expression, destinationBuffer, plan, useProviderForValue);
            
        case EagleDbSqlExpressionTypeCastExpression:
            return EagleDbSqlExpression_CompilePlanIntoBuffer_Cast_(expression, destinationBuffer, plan);
            
        case EagleDbSqlExpressionTypeSelect:
            return 0;
            
    }
}

void EagleDbSqlExpression_CompilePlan(EagleDbSqlExpression **expressions, int totalExpressions, int whereClause, EaglePlan *plan)
{
    int i, *results, destinationBuffer;
    
    if(NULL == plan) {
        return;
    }
    
    /* for now we will just assume we don't need more than 20 buffers */
    EaglePlan_prepareBuffers(plan, 20);
    
    /* make sure we don't override buffers that are already assigned by providers */
    destinationBuffer = 0;
    
    EagleLinkedList_Foreach(plan->providers, EaglePlanBufferProvider*, provider)
    {
        if(provider->destinationBuffer >= destinationBuffer) {
            destinationBuffer = provider->destinationBuffer + 1;
        }
        
        /* each provider will go into a page, sync their types */
        plan->bufferTypes[provider->destinationBuffer] = provider->value.provider.provider->type;
    }
    EagleLinkedList_ForeachEnd
    
    /* prepare result providers */
    plan->resultFields = totalExpressions;
    plan->result = (EaglePageProvider**) EagleMemory_MultiAllocate("EagleDbSqlExpression_CompilePlan.1", sizeof(EaglePageProvider*), plan->resultFields);
    
    /* compile expressions */
    results = (int*) EagleMemory_MultiAllocate("EagleDbSqlExpression_CompilePlan.2", sizeof(int), totalExpressions);
    for(i = 0; i < totalExpressions; ++i) {
        char *desc;
        EaglePageProvider *provider;
        EagleDataType dataType;
        
        results[i] = EagleDbSqlExpression_CompilePlanIntoBuffer_(expressions[i], &destinationBuffer, plan, EagleTrue);
        
        if(EagleTrue == EaglePlan_isError(plan)) {
            EagleMemory_Free(results);
            return;
        }
        
        /*
         since we don't know the type of result providers until after we evaluate the expression we can now setup the
         result provider now
         */
        desc = EagleDbSqlExpression_toString(expressions[i]);
        dataType = plan->bufferTypes[results[i]];
        provider = (EaglePageProvider*) EaglePageProviderStream_New(dataType, plan->pageSize, desc);
        plan->result[i] = provider;
        EaglePlan_addFreeObject(plan, provider, (void(*)(void*)) EaglePageProvider_Delete);
        EagleMemory_Free(desc);
    }
    
    /* if there is a WHERE clause expression only send those records */
    for(i = 0; i < totalExpressions; ++i) {
        EaglePlanOperation *epo;
        char msg[1024];
        
        if(i != whereClause) {
            if(whereClause >= 0) {
                /* send some result data to the provider */
                char *t1, *t2, *t3;
                
                t1 = EagleDataType_typeToName(plan->bufferTypes[results[whereClause]]);
                t2 = EagleDataType_typeToName(plan->bufferTypes[results[i]]);
                t3 = EagleDataType_typeToName(plan->result[i]->type);
                
                sprintf(msg, "WHERE <%d> (%s), send <%d> (%s) to provider <%d> (%s)", results[whereClause], t1, results[i], t2, i, t3);
                epo = EaglePlanOperation_NewWithPage(EaglePageOperations_SendPageToProvider, -1, results[whereClause], results[i], plan->result[i], EagleFalse, msg);
                EaglePlan_addOperation(plan, epo);
                EaglePlan_addFreeObject(plan, epo, (void(*)(void*)) EaglePlanOperation_Delete);
                
                EagleMemory_Free(t1);
                EagleMemory_Free(t2);
                EagleMemory_Free(t3);
            }
            else {
                char *t1, *t2;
                
                t1 = EagleDataType_typeToName(plan->bufferTypes[results[i]]);
                t2 = EagleDataType_typeToName(plan->result[i]->type);
                
                /* send all the result data to the provider */
                sprintf(msg, "ALL <%d> (%s) to provider <%d> (%s)", results[i], t1, i, t2);
                epo = EaglePlanOperation_NewWithPage(EaglePageOperations_SendPageToProvider, -1, -1, results[i], plan->result[i], EagleFalse, msg);
                EaglePlan_addOperation(plan, epo);
                EaglePlan_addFreeObject(plan, epo, (void(*)(void*)) EaglePlanOperation_Delete);
                
                EagleMemory_Free(t1);
                EagleMemory_Free(t2);
            }
        }
    }
    
    EagleMemory_Free(results);
}

void EagleDbSqlExpression_Delete(EagleDbSqlExpression *expr)
{
    if(NULL == expr) {
        return;
    }
    
    switch(expr->expressionType) {
            
        case EagleDbSqlExpressionTypeUnaryExpression:
            EagleDbSqlUnaryExpression_Delete((EagleDbSqlUnaryExpression*) expr);
            break;
            
        case EagleDbSqlExpressionTypeBinaryExpression:
            EagleDbSqlBinaryExpression_Delete((EagleDbSqlBinaryExpression*) expr);
            break;
            
        case EagleDbSqlExpressionTypeSelect:
            EagleDbSqlSelect_Delete((EagleDbSqlSelect*) expr);
            break;
            
        case EagleDbSqlExpressionTypeValue:
            EagleDbSqlValue_Delete((EagleDbSqlValue*) expr);
            break;
            
        case EagleDbSqlExpressionTypeFunctionExpression:
            EagleDbSqlFunctionExpression_Delete((EagleDbSqlFunctionExpression*) expr);
            break;
            
        case EagleDbSqlExpressionTypeCastExpression:
            EagleDbSqlCastExpression_Delete((EagleDbSqlCastExpression*) expr);
            break;
            
    }
}

void EagleDbSqlExpression_DeleteRecursive(EagleDbSqlExpression *expr)
{
    if(NULL == expr) {
        return;
    }
    
    switch(expr->expressionType) {
            
        case EagleDbSqlExpressionTypeUnaryExpression:
            EagleDbSqlUnaryExpression_DeleteRecursive((EagleDbSqlUnaryExpression*) expr);
            break;
            
        case EagleDbSqlExpressionTypeBinaryExpression:
            EagleDbSqlBinaryExpression_DeleteRecursive((EagleDbSqlBinaryExpression*) expr);
            break;
            
        case EagleDbSqlExpressionTypeSelect:
            EagleDbSqlSelect_DeleteRecursive((EagleDbSqlSelect*) expr);
            break;
            
        case EagleDbSqlExpressionTypeValue:
            EagleDbSqlValue_Delete((EagleDbSqlValue*) expr);
            break;
            
        case EagleDbSqlExpressionTypeFunctionExpression:
            EagleDbSqlFunctionExpression_DeleteRecursive((EagleDbSqlFunctionExpression*) expr);
            break;
            
        case EagleDbSqlExpressionTypeCastExpression:
            EagleDbSqlCastExpression_DeleteRecursive((EagleDbSqlCastExpression*) expr);
            break;
            
    }
}

char* EagleDbSqlExpression_toString(EagleDbSqlExpression *expr)
{
    if(NULL == expr) {
        return strdup("");
    }
    
    switch(expr->expressionType) {
            
        case EagleDbSqlExpressionTypeUnaryExpression:
            return EagleDbSqlUnaryExpression_toString((EagleDbSqlUnaryExpression*) expr);
            
        case EagleDbSqlExpressionTypeBinaryExpression:
            return EagleDbSqlBinaryExpression_toString((EagleDbSqlBinaryExpression*) expr);
            
        case EagleDbSqlExpressionTypeSelect:
            return EagleDbSqlSelect_toString((EagleDbSqlSelect*) expr);
            
        case EagleDbSqlExpressionTypeValue:
            return EagleDbSqlValue_toString((EagleDbSqlValue*) expr);
            
        case EagleDbSqlExpressionTypeFunctionExpression:
            return EagleDbSqlFunctionExpression_toString((EagleDbSqlFunctionExpression*) expr);
            
        case EagleDbSqlExpressionTypeCastExpression:
            return EagleDbSqlCastExpression_toString((EagleDbSqlCastExpression*) expr);
            
    }
}
