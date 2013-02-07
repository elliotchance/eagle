#include <stdlib.h>
#include <stdio.h>
#include "EagleDbSqlUnaryExpression.h"
#include "EagleMemory.h"

EagleDbSqlUnaryExpression* EagleDbSqlUnaryExpression_New(EagleDbSqlUnaryExpressionOperator op, EagleDbSqlExpression *expr)
{
    EagleDbSqlUnaryExpression *exp = (EagleDbSqlUnaryExpression*) EagleMemory_Allocate("EagleDbSqlUnaryExpression_New.1", sizeof(EagleDbSqlUnaryExpression));
    if(NULL == exp) {
        return NULL;
    }
    
    exp->expressionType = EagleDbSqlExpressionTypeUnaryExpression;
    exp->expr = expr;
    exp->op = op;
    
    return exp;
}

void EagleDbSqlUnaryExpression_Delete(EagleDbSqlUnaryExpression *expr)
{
    if(NULL == expr) {
        return;
    }
    
    EagleMemory_Free(expr);
}

void EagleDbSqlUnaryExpression_DeleteRecursive(EagleDbSqlUnaryExpression *expr)
{
    if(NULL == expr) {
        return;
    }
    
    EagleDbSqlExpression_DeleteRecursive((EagleDbSqlExpression*) expr->expr);
    EagleMemory_Free(expr);
}

char* EagleDbSqlUnaryExpression_toString(EagleDbSqlUnaryExpression *expr)
{
    char* s = (char*) EagleMemory_Allocate("EagleDbSqlUnaryExpression_toString.1", 1024), *exp;
    char *beforeOp = NULL, *afterOp = NULL;
    if(NULL == s) {
        return NULL;
    }
    
    EagleDbSqlUnaryExpressionOperator_toString(expr->op, &beforeOp, &afterOp);
    exp = EagleDbSqlExpression_toString(expr->expr);
    
    sprintf(s, "%s%s%s", beforeOp, exp, afterOp);
    
    EagleMemory_Free(beforeOp);
    EagleMemory_Free(afterOp);
    EagleMemory_Free(exp);
    return s;
}
