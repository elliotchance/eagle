#include <stdlib.h>
#include <stdio.h>
#include "EagleDbSqlBinaryExpression.h"
#include "EagleMemory.h"

/**
 Create a new EagleDbSqlBinaryExpression.
 
 @param [in] left Left operand.
 @param [in] op Operator.
 @param [in] right Right operand.
 */
EagleDbSqlBinaryExpression* EagleDbSqlBinaryExpression_New(EagleDbSqlExpression *left, EagleDbSqlExpressionOperator op, EagleDbSqlExpression *right)
{
    EagleDbSqlBinaryExpression *expr = (EagleDbSqlBinaryExpression*) malloc(sizeof(EagleDbSqlBinaryExpression));
    
    expr->expressionType = EagleDbSqlExpressionTypeBinaryExpression;
    expr->left = left;
    expr->op = op;
    expr->right = right;
    
    return expr;
}

void EagleDbSqlBinaryExpression_Delete(EagleDbSqlBinaryExpression *expr)
{
    EagleDbSqlExpression_Delete(expr->left);
    EagleDbSqlExpression_Delete(expr->right);
    EagleMemory_Free(expr);
}

char* EagleDbSqlBinaryExpression_toString(EagleDbSqlBinaryExpression *expr)
{
    char* s = (char*) malloc(1024);
    
    char *left = EagleDbSqlExpression_toString(expr->left);
    char *right = EagleDbSqlExpression_toString(expr->right);
    char *op = EagleDbSqlExpressionOperator_toString(expr->op);
    sprintf(s, "%s %s %s", left, op, right);
    EagleMemory_Free(left);
    EagleMemory_Free(right);
    EagleMemory_Free(op);
    
    return s;
}
