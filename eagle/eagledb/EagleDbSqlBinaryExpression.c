#include <stdlib.h>
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
