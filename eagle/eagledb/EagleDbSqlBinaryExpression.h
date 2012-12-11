#ifndef eagle_EagleDbSqlBinaryExpression_h
#define eagle_EagleDbSqlBinaryExpression_h

#include "EagleDbSqlExpression.h"
#include "EagleDbSqlExpressionOperator.h"

/**
 Expression type is EagleDbSqlExpressionTypeBinaryExpression.
 
 @see EagleDbSqlExpressionType
 */
typedef struct {
    
    EagleDbSqlExpressionHeader;
    
    /**
     The left operand.
     */
    EagleDbSqlExpression *left;
    
    /**
     The operator between the \p left and \p right operands.
     */
    EagleDbSqlExpressionOperator op;
    
    /**
     The right operand.
     */
    EagleDbSqlExpression *right;
    
} EagleDbSqlBinaryExpression;

EagleDbSqlBinaryExpression* EagleDbSqlBinaryExpression_New(EagleDbSqlExpression *left, EagleDbSqlExpressionOperator op, EagleDbSqlExpression *right);

#endif
