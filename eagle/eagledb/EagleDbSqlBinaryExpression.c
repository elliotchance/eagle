#include <stdlib.h>
#include <stdio.h>
#include "EagleDbSqlBinaryExpression.h"
#include "EagleMemory.h"

EagleDbSqlBinaryExpression* EagleDbSqlBinaryExpression_New(EagleDbSqlExpression *left, EagleDbSqlExpressionOperator op, EagleDbSqlExpression *right)
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

void EagleDbSqlBinaryExpression_Delete(EagleDbSqlBinaryExpression *expr, EagleBoolean recursive)
{
    if(EagleTrue == recursive) {
        EagleDbSqlExpression_Delete((EagleDbSqlExpression*) expr->left, recursive);
        EagleDbSqlExpression_Delete((EagleDbSqlExpression*) expr->right, recursive);
    }
    EagleMemory_Free(expr);
}

char* EagleDbSqlBinaryExpression_toString(EagleDbSqlBinaryExpression *expr)
{
    char* s = (char*) EagleMemory_Allocate("EagleDbSqlBinaryExpression_toString.1", 1024), *left, *right, *op;
    if(NULL == s) {
        return NULL;
    }
    
    left = EagleDbSqlExpression_toString(expr->left);
    right = EagleDbSqlExpression_toString(expr->right);
    op = EagleDbSqlExpressionOperator_toString(expr->op);
    sprintf(s, "%s %s %s", left, op, right);
    EagleMemory_Free(left);
    EagleMemory_Free(right);
    EagleMemory_Free(op);
    
    return s;
}
