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

EagleBoolean EagleDbSqlBinaryExpression_GetOperation(EagleDataType left,
                                                     EagleDbSqlBinaryExpressionOperator op,
                                                     EagleDataType right,
                                                     EagleDbSqlBinaryOperator *match)
{
    unsigned long i;
    static EagleDbSqlBinaryOperator ops[] = {
        /* Integer                    left     operator          right    function              returns */
        EagleDbSqlBinaryOperator_Make(Integer, Plus,             Integer, AdditionPage,         Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Equals,           Integer, EqualsPage,           Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Modulus,          Integer, ModulusPage,          Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Multiply,         Integer, MultiplyIntPage,      Integer),
        EagleDbSqlBinaryOperator_Make(Integer, NotEquals,        Integer, NotEqualsPage,        Integer),
        EagleDbSqlBinaryOperator_Make(Integer, GreaterThan,      Integer, GreaterThanPage,      Integer),
        EagleDbSqlBinaryOperator_Make(Integer, LessThan,         Integer, LessThanPage,         Integer),
        EagleDbSqlBinaryOperator_Make(Integer, GreaterThanEqual, Integer, GreaterThanEqualPage, Integer),
        EagleDbSqlBinaryOperator_Make(Integer, LessThanEqual,    Integer, LessThanEqualPage,    Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Minus,            Integer, SubtractPage,         Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Divide,           Integer, DividePage,           Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Or,               Integer, OrPage,               Integer),
        EagleDbSqlBinaryOperator_Make(Integer, And,              Integer, AndPage,              Integer),
        
        /* Float                      left   operator  right  function           returns */
        EagleDbSqlBinaryOperator_Make(Float, Multiply, Float, MultiplyFloatPage, Float),
    };
    
    for(i = 0; i < sizeof(ops) / sizeof(EagleDbSqlBinaryOperator); ++i) {
        if(left == ops[i].left && right == ops[i].right && op == ops[i].op) {
            *match = ops[i];
            return EagleTrue;
        }
    }
    
    return EagleFalse;
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
