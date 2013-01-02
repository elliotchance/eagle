#include <stdlib.h>
#include <stdio.h>
#include "EagleDbSqlBinaryExpression.h"

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
    free(expr);
}

char* EagleDbSqlBinaryExpression_toString(EagleDbSqlBinaryExpression *expr)
{
    char* s = (char*) malloc(1024);
    
    char *left = EagleDbSqlExpression_toString(expr->left);
    char *right = EagleDbSqlExpression_toString(expr->right);
    char *op = EagleDbSqlExpressionOperator_toString(expr->op);
    sprintf(s, "%s %s %s", left, op, right);
    free(left);
    free(right);
    free(op);
    
    return s;
}
