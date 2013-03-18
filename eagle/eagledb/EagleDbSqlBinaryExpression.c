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

EagleBoolean EagleDbSqlBinaryExpression_GetRightOperation(EagleDataType right,
                                                          EagleDbSqlBinaryExpressionOperator op,
                                                          EagleDbSqlBinaryOperator *match)
{
    unsigned long i;
    static EagleDbSqlBinaryOperator ops[] = {
        /* Integer                    left     operator          right    function              returns */
        EagleDbSqlBinaryOperator_Make(Integer, LessThan,         Integer, GreaterThanInt,       Integer),
        EagleDbSqlBinaryOperator_Make(Integer, GreaterThan,      Integer, LessThanInt,          Integer),
        EagleDbSqlBinaryOperator_Make(Integer, LessThanEqual,    Integer, GreaterThanEqualsInt, Integer),
        EagleDbSqlBinaryOperator_Make(Integer, GreaterThanEqual, Integer, LessThanEqualsInt,    Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Equals,           Integer, EqualsInt,            Integer),
        EagleDbSqlBinaryOperator_Make(Integer, NotEquals,        Integer, NotEqualsInt,         Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Plus,             Integer, AdditionInt,          Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Multiply,         Integer, MultiplyInt,          Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Modulus,          Integer, ModulusRightInt,      Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Divide,           Integer, DivideRightInt,       Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Minus,            Integer, SubtractRightInt,     Integer),
    };
    
    for(i = 0; i < sizeof(ops) / sizeof(EagleDbSqlBinaryOperator); ++i) {
        if(right == ops[i].right && op == ops[i].op) {
            *match = ops[i];
            return EagleTrue;
        }
    }
    
    return EagleFalse;
}

EagleBoolean EagleDbSqlBinaryExpression_GetLeftOperation(EagleDataType left,
                                                         EagleDbSqlBinaryExpressionOperator op,
                                                         EagleDbSqlBinaryOperator *match)
{
    unsigned long i;
    static EagleDbSqlBinaryOperator ops[] = {
        /* Integer                    left     operator          right    function              returns */
        EagleDbSqlBinaryOperator_Make(Integer, GreaterThan,      Integer, GreaterThanInt,       Integer),
        EagleDbSqlBinaryOperator_Make(Integer, LessThan,         Integer, LessThanInt,          Integer),
        EagleDbSqlBinaryOperator_Make(Integer, GreaterThanEqual, Integer, GreaterThanEqualsInt, Integer),
        EagleDbSqlBinaryOperator_Make(Integer, LessThanEqual,    Integer, LessThanEqualsInt,    Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Equals,           Integer, EqualsInt,            Integer),
        EagleDbSqlBinaryOperator_Make(Integer, NotEquals,        Integer, NotEqualsInt,         Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Plus,             Integer, AdditionInt,          Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Multiply,         Integer, MultiplyInt,          Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Minus,            Integer, SubtractLeftInt,      Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Divide,           Integer, DivideLeftInt,        Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Modulus,          Integer, ModulusLeftInt,       Integer),
    };
    
    for(i = 0; i < sizeof(ops) / sizeof(EagleDbSqlBinaryOperator); ++i) {
        if(left == ops[i].left && op == ops[i].op) {
            *match = ops[i];
            return EagleTrue;
        }
    }
    
    return EagleFalse;
}

EagleBoolean EagleDbSqlBinaryExpression_GetPageOperation(EagleDataType left,
                                                         EagleDbSqlBinaryExpressionOperator op,
                                                         EagleDataType right,
                                                         EagleDbSqlBinaryOperator *match)
{
    unsigned long i;
    static EagleDbSqlBinaryOperator ops[] = {
        /* Integer                    left     operator          right    function                 returns */
        EagleDbSqlBinaryOperator_Make(Integer, Plus,             Integer, AdditionPageInt,         Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Equals,           Integer, EqualsPageInt,           Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Modulus,          Integer, ModulusPageInt,          Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Multiply,         Integer, MultiplyPageInt,         Integer),
        EagleDbSqlBinaryOperator_Make(Integer, NotEquals,        Integer, NotEqualsPageInt,        Integer),
        EagleDbSqlBinaryOperator_Make(Integer, GreaterThan,      Integer, GreaterThanPageInt,      Integer),
        EagleDbSqlBinaryOperator_Make(Integer, LessThan,         Integer, LessThanPageInt,         Integer),
        EagleDbSqlBinaryOperator_Make(Integer, GreaterThanEqual, Integer, GreaterThanEqualPageInt, Integer),
        EagleDbSqlBinaryOperator_Make(Integer, LessThanEqual,    Integer, LessThanEqualPageInt,    Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Minus,            Integer, SubtractPageInt,         Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Divide,           Integer, DividePageInt,           Integer),
        EagleDbSqlBinaryOperator_Make(Integer, Or,               Integer, OrPageInt,               Integer),
        EagleDbSqlBinaryOperator_Make(Integer, And,              Integer, AndPageInt,              Integer),
        
        /* Float                      left   operator          right  function                   returns */
        EagleDbSqlBinaryOperator_Make(Float, Multiply,         Float, MultiplyPageFloat,         Float),
        EagleDbSqlBinaryOperator_Make(Float, Plus,             Float, AdditionPageFloat,         Float),
        EagleDbSqlBinaryOperator_Make(Float, Equals,           Float, EqualsPageFloat,           Float),
        EagleDbSqlBinaryOperator_Make(Float, NotEquals,        Float, NotEqualsPageFloat,        Float),
        EagleDbSqlBinaryOperator_Make(Float, GreaterThan,      Float, GreaterThanPageFloat,      Float),
        EagleDbSqlBinaryOperator_Make(Float, LessThan,         Float, LessThanPageFloat,         Float),
        EagleDbSqlBinaryOperator_Make(Float, GreaterThanEqual, Float, GreaterThanEqualPageFloat, Float),
        EagleDbSqlBinaryOperator_Make(Float, LessThanEqual,    Float, LessThanEqualPageFloat,    Float),
        EagleDbSqlBinaryOperator_Make(Float, Minus,            Float, SubtractPageFloat,         Float),
        EagleDbSqlBinaryOperator_Make(Float, Divide,           Float, DividePageFloat,           Float)
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
