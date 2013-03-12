#ifndef eagle_EagleDbSqlCastExpression_h
#define eagle_EagleDbSqlCastExpression_h

#include "EagleDbSqlExpression.h"
#include "Eagle.h"

/**
 Expression type is EagleDbSqlExpressionTypeCastExpression.
 
 @see EagleDbSqlExpressionType
 */
typedef struct {
    
    /**
     Header.
     @see EagleDbSqlExpressionHeader
     */
    EagleDbSqlExpressionHeader;
    
    /**
     The operand.
     */
    EAGLE_ATTR_PROVIDED EagleDbSqlExpression *expr;
    
    /**
     The type to cast to.
     */
    EAGLE_ATTR_NA EagleDataType castAs;
    
} EagleDbSqlCastExpression;

/**
 Used by EagleDbSqlCastExpression_GetOperation() to find the appropriate page operation for an cast operation.
 */
typedef struct {
    
    /**
     Data type of the left side (first operand).
     */
    EagleDataType left;
    
    /**
     Data type of the right side (second operand).
     */
    EagleDataType right;
    
    /**
     The page operation function.
     */
    EaglePageOperationFunction(func);
    
} EagleDbSqlCastOperator;

/**
 * Create a new EagleDbSqlCastExpression.
 * @param [in] expr Expression.
 * @param [in] castAs Cast expression to data type.
 * @return A new instance.
 */
EagleDbSqlCastExpression* EagleDbSqlCastExpression_New(EagleDbSqlExpression *expr, EagleDataType castAs);

/**
 * Free a cast expression.
 * @note It is safer to use EagleDbSqlExpression_Delete() since it will automatically use the correct Delete function.
 * @param [in] expr The instance.
 */
void EagleDbSqlCastExpression_Delete(EagleDbSqlCastExpression *expr);

/**
 * Free a cast expression (recursively).
 * @note It is safer to use EagleDbSqlExpression_DeleteRecursive() since it will automatically use the correct Delete
 *       function.
 * @param [in] expr The instance.
 */
void EagleDbSqlCastExpression_DeleteRecursive(EagleDbSqlCastExpression *expr);

/**
 * Render a cast expression into a string.
 * @param [in] expr The instance.
 * @return A new string representation of the expression.
 */
char* EagleDbSqlCastExpression_toString(EagleDbSqlCastExpression *expr);

/**
 Find the appropriate page operation for a cast expression.
 
 @param [in] left Left data type (first operand).
 @param [in] right Right data type (second operand).
 @param [out] match If a match is found it will be copied into this output parameter.
 @return EagleTrue if the operator can be found.
 */
EagleBoolean EagleDbSqlCastExpression_GetOperation(EagleDataType left,
                                                   EagleDataType right,
                                                   EagleDbSqlCastOperator *match);

#endif
