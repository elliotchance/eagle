#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EagleDbSqlFunctionExpression.h"
#include "EagleMemory.h"

EagleDbSqlFunctionExpression* EagleDbSqlFunctionExpression_New(char *name, EagleDbSqlExpression *expr)
{
    EagleDbSqlFunctionExpression *exp = (EagleDbSqlFunctionExpression*) EagleMemory_Allocate("EagleDbSqlFunctionExpression_New.1", sizeof(EagleDbSqlFunctionExpression));
    if(NULL == exp) {
        return NULL;
    }
    
    exp->expressionType = EagleDbSqlExpressionTypeFunctionExpression;
    exp->expr = expr;
    exp->name = strdup(name);
    
    return exp;
}

void EagleDbSqlFunctionExpression_Delete(EagleDbSqlFunctionExpression *expr)
{
    if(NULL == expr) {
        return;
    }
    
    EagleMemory_Free(expr->name);
    EagleMemory_Free(expr);
}

void EagleDbSqlFunctionExpression_DeleteRecursive(EagleDbSqlFunctionExpression *expr)
{
    if(NULL == expr) {
        return;
    }
    
    EagleMemory_Free(expr->name);
    EagleDbSqlExpression_DeleteRecursive((EagleDbSqlExpression*) expr->expr);
    EagleMemory_Free(expr);
}

char* EagleDbSqlFunctionExpression_toString(EagleDbSqlFunctionExpression *expr)
{
    char* s = (char*) EagleMemory_Allocate("EagleDbSqlFunctionExpression_toString.1", 1024), *exp;
    if(NULL == s) {
        return NULL;
    }
    
    exp = EagleDbSqlExpression_toString(expr->expr);
    
    sprintf(s, "%s(%s)", expr->name, exp);
    
    EagleMemory_Free(exp);
    return s;
}
