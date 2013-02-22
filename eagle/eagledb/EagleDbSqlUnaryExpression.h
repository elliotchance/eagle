#ifndef eagle_EagleDbSqlUnaryExpression_h
#define eagle_EagleDbSqlUnaryExpression_h

#include "EagleDbSqlExpression.h"
#include "EagleDbSqlUnaryExpressionOperator.h"
#include "Eagle.h"

/**
 Expression type is EagleDbSqlExpressionTypeUnaryExpression.
 
 @see EagleDbSqlExpressionType
 */
typedef struct {
    
    /**
     Header.
     @see EagleDbSqlExpressionHeader
     */
    EagleDbSqlExpressionHeader;
    
    /**
     The operator.
     */
    EAGLE_ATTR_NA EagleDbSqlUnaryExpressionOperator op;
    
    /**
     The operand.
     */
    EAGLE_ATTR_PROVIDED EagleDbSqlExpression *expr;
    
} EagleDbSqlUnaryExpression;

/**
 * Create a new EagleDbSqlUnaryExpression.
 * @param [in] op Operator.
 * @param [in] expr Expression.
 * @return A new instance.
 */
EagleDbSqlUnaryExpression* EagleDbSqlUnaryExpression_New(EagleDbSqlUnaryExpressionOperator op, EagleDbSqlExpression *expr);

/**
 * Free a unary expression.
 * @note It is safer to use EagleDbSqlExpression_Delete() since it will automatically use the correct Delete function.
 * @param [in] expr The instance.
 */
void EagleDbSqlUnaryExpression_Delete(EagleDbSqlUnaryExpression *expr);

/**
 * Free a unary expression (recursively).
 * @note It is safer to use EagleDbSqlExpression_DeleteRecursive() since it will automatically use the correct Delete
 *       function.
 * @param [in] expr The instance.
 */
void EagleDbSqlUnaryExpression_DeleteRecursive(EagleDbSqlUnaryExpression *expr);

/**
 * Render a unary expression into a string.
 * @param [in] expr The instance.
 * @return A new string representation of the expression.
 */
char* EagleDbSqlUnaryExpression_toString(EagleDbSqlUnaryExpression *expr);

#endif