#ifndef eagle_EagleDbSqlBinaryExpression_h
#define eagle_EagleDbSqlBinaryExpression_h

#include "EagleDbSqlExpression.h"
#include "EagleDbSqlBinaryExpressionOperator.h"
#include "Eagle.h"
#include "EaglePageOperations.h"
#include "EagleDataType.h"

/**
 Expression type is EagleDbSqlExpressionTypeBinaryExpression.
 
 @see EagleDbSqlExpressionType
 */
typedef struct {
    
    /**
     Header.
     @see EagleDbSqlExpressionHeader
     */
    EagleDbSqlExpressionHeader;
    
    /**
     The left operand.
     */
    EAGLE_ATTR_PROVIDED EagleDbSqlExpression *left;
    
    /**
     The operator between the \p left and \p right operands.
     */
    EAGLE_ATTR_NA EagleDbSqlBinaryExpressionOperator op;
    
    /**
     The right operand.
     */
    EAGLE_ATTR_PROVIDED EagleDbSqlExpression *right;
    
} EagleDbSqlBinaryExpression;

/**
 * Create a new EagleDbSqlBinaryExpression.
 * @param [in] left Left operand.
 * @param [in] op Operator.
 * @param [in] right Right operand.
 * @return A new instance.
 */
EagleDbSqlBinaryExpression* EagleDbSqlBinaryExpression_New(EagleDbSqlExpression *left, EagleDbSqlBinaryExpressionOperator op, EagleDbSqlExpression *right);

/**
 * Free a binary expression.
 * @note It is safer to use EagleDbSqlExpression_Delete() since it will automatically use the correct Delete function.
 * @param [in] expr The instance.
 */
void EagleDbSqlBinaryExpression_Delete(EagleDbSqlBinaryExpression *expr);

/**
 * Free a binary expression (recursively).
 * @note It is safer to use EagleDbSqlExpression_DeleteRecursive() since it will automatically use the correct Delete
 *       function.
 * @param [in] expr The instance.
 */
void EagleDbSqlBinaryExpression_DeleteRecursive(EagleDbSqlBinaryExpression *expr);

/**
 * Render a binary expression into a string.
 * @param [in] expr The instance.
 * @return A new string representation of the expression.
 */
char* EagleDbSqlBinaryExpression_toString(EagleDbSqlBinaryExpression *expr);

EaglePageOperationFunction(EagleDbSqlBinaryExpression_GetOperation(EagleDataType left,
                                                                   EagleDbSqlBinaryExpressionOperator op,
                                                                   EagleDataType right,
                                                                   char **error));

#endif
