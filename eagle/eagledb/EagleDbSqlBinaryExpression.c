#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EagleDbSqlBinaryExpression.h"
#include "EagleMemory.h"

EagleDbSqlBinaryExpression* EagleDbSqlBinaryExpression_New(EagleDbSqlExpression *left,
                                                           EagleDbSqlBinaryExpressionOperator op,
                                                           EagleDbSqlExpression *right)
{
    EagleDbSqlBinaryExpression *expr = (EagleDbSqlBinaryExpression*) EagleMemory_Allocate("EagleDbSqlBinaryExpression_New.1", sizeof(EagleDbSqlBinaryExpression));
    if(NULL == expr) {
        return NULL;
    }
    
    expr->expressionType = EagleDbSqlExpressionTypeBinaryExpression;
    expr->left = left;
    expr->op = op;
    expr->right = right;
    
    return expr;
}

EaglePageOperationFunction(EagleDbSqlBinaryExpression_GetOperation(EagleDataType left,
                                                                   EagleDbSqlBinaryExpressionOperator op,
                                                                   EagleDataType right,
                                                                   char **error))
{
    EaglePageOperationFunction(pageOperation) = NULL;
    
    if(left != right) {
        *error = strdup("You cannot mix data types, both operands must be same data type.");
        return NULL;
    }
    
    switch(left) {
            
        case EagleDataTypeVarchar:
        case EagleDataTypeUnknown:
            *error = strdup("No operators for data type.");
            return NULL;
            
        case EagleDataTypeInteger:
            
            switch(op) {
                    
                case EagleDbSqlBinaryExpressionOperatorPlus:
                    pageOperation = EaglePageOperations_AdditionPage;
                    break;
                    
                case EagleDbSqlBinaryExpressionOperatorEquals:
                    pageOperation = EaglePageOperations_EqualsPage;
                    break;
                    
                case EagleDbSqlBinaryExpressionOperatorModulus:
                    pageOperation = EaglePageOperations_ModulusPage;
                    break;
                    
                case EagleDbSqlBinaryExpressionOperatorMultiply:
                    pageOperation = EaglePageOperations_MultiplyIntPage;
                    break;
                    
                case EagleDbSqlBinaryExpressionOperatorNotEquals:
                    pageOperation = EaglePageOperations_NotEqualsPage;
                    break;
                    
                case EagleDbSqlBinaryExpressionOperatorGreaterThan:
                    pageOperation = EaglePageOperations_GreaterThanPage;
                    break;
                    
                case EagleDbSqlBinaryExpressionOperatorLessThan:
                    pageOperation = EaglePageOperations_LessThanPage;
                    break;
                    
                case EagleDbSqlBinaryExpressionOperatorGreaterThanEqual:
                    pageOperation = EaglePageOperations_GreaterThanEqualPage;
                    break;
                    
                case EagleDbSqlBinaryExpressionOperatorLessThanEqual:
                    pageOperation = EaglePageOperations_LessThanEqualPage;
                    break;
                    
                case EagleDbSqlBinaryExpressionOperatorMinus:
                    pageOperation = EaglePageOperations_SubtractPage;
                    break;
                    
                case EagleDbSqlBinaryExpressionOperatorDivide:
                    pageOperation = EaglePageOperations_DividePage;
                    break;
                    
                case EagleDbSqlBinaryExpressionOperatorOr:
                    pageOperation = EaglePageOperations_OrPage;
                    break;
                    
                case EagleDbSqlBinaryExpressionOperatorAnd:
                    pageOperation = EaglePageOperations_AndPage;
                    break;
                    
            }
            break;
            
        case EagleDataTypeFloat:
            switch(op) {
                    
                case EagleDbSqlBinaryExpressionOperatorPlus:
                case EagleDbSqlBinaryExpressionOperatorEquals:
                case EagleDbSqlBinaryExpressionOperatorModulus:
                case EagleDbSqlBinaryExpressionOperatorNotEquals:
                case EagleDbSqlBinaryExpressionOperatorGreaterThan:
                case EagleDbSqlBinaryExpressionOperatorLessThan:
                case EagleDbSqlBinaryExpressionOperatorGreaterThanEqual:
                case EagleDbSqlBinaryExpressionOperatorLessThanEqual:
                case EagleDbSqlBinaryExpressionOperatorMinus:
                case EagleDbSqlBinaryExpressionOperatorDivide:
                case EagleDbSqlBinaryExpressionOperatorOr:
                case EagleDbSqlBinaryExpressionOperatorAnd:
                    pageOperation = NULL;
                    break;
                    
                case EagleDbSqlBinaryExpressionOperatorMultiply:
                    pageOperation = EaglePageOperations_MultiplyFloatPage;
                    break;
                    
            }
            break;
            
    }
    
    if(NULL == pageOperation) {
        *error = strdup("Data type unsupported for operators.");
        return NULL;
    }
    
    return pageOperation;
}

void EagleDbSqlBinaryExpression_Delete(EagleDbSqlBinaryExpression *expr)
{
    if(NULL == expr) {
        return;
    }
    
    EagleMemory_Free(expr);
}

void EagleDbSqlBinaryExpression_DeleteRecursive(EagleDbSqlBinaryExpression *expr)
{
    if(NULL == expr) {
        return;
    }
    
    EagleDbSqlExpression_DeleteRecursive((EagleDbSqlExpression*) expr->left);
    EagleDbSqlExpression_DeleteRecursive((EagleDbSqlExpression*) expr->right);
    EagleMemory_Free(expr);
}

char* EagleDbSqlBinaryExpression_toString(EagleDbSqlBinaryExpression *expr)
{
    char* s = (char*) EagleMemory_Allocate("EagleDbSqlBinaryExpression_toString.1", 1024), *left, *right, *op;
    if(NULL == s) {
        return NULL;
    }
    
    left = EagleDbSqlExpression_toString(expr->left);
    op = EagleDbSqlBinaryExpressionOperator_toString(expr->op);
    right = EagleDbSqlExpression_toString(expr->right);
    
    sprintf(s, "%s %s %s", left, op, right);
    
    EagleMemory_Free(left);
    EagleMemory_Free(op);
    EagleMemory_Free(right);
    return s;
}
