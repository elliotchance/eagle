#include <stdlib.h>
#include <stdio.h>
#include "EagleDbSqlBinaryExpression.h"

/**
 Create a new EagleDbSqlBinaryExpression.
 
 @param left Left operand.
 @param op Operator.
 @param right Right operand.
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
    sprintf(s, "%s %s %s", EagleDbSqlExpression_toString(expr->left), EagleDbSqlExpressionOperator_toString(expr->op), EagleDbSqlExpression_toString(expr->right));
    return s;
}
