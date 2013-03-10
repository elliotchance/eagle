#ifndef eagle_EagleDbSqlUnaryExpression_h
#define eagle_EagleDbSqlUnaryExpression_h

#include "EagleDbSqlExpression.h"
#include "EagleDbSqlUnaryExpressionOperator.h"
#include "Eagle.h"

/**
 This is used by EagleDbSqlUnaryExpression_GetOperation()
 */
#define EagleDbSqlUnaryOperator_Make(op, right, func, returnType) { EagleDbSqlUnaryExpressionOperator##op, EagleDataType##right, EaglePageOperations_##func, EagleDataType##returnType }

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
 Used by EagleDbSqlUnaryExpression_GetOperation() to find the appropriate page operation for an operator.
 */
typedef struct {
    
    /**
     The operator.
     */
    EagleDbSqlUnaryExpressionOperator op;
    
    /**
     Data type of the right side (first operand).
     */
    EagleDataType right;
    
    /**
     The page operation function.
     */
    EaglePageOperationFunction(func);
    
    /**
     The return type that the page operation function outputs.
     */
    EagleDataType returnType;
    
} EagleDbSqlUnaryOperator;

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

/**
 Find the appropriate page operation for an operator.
 
 @param [in] op The operator.
 @param [in] right Right data type (first operand).
 @param [out] match If a match is found it will be copied into this output parameter.
 @return EagleTrue if the operator can be found.
 */
EagleBoolean EagleDbSqlUnaryExpression_GetOperation(EagleDbSqlUnaryExpressionOperator op,
                                                    EagleDataType right,
                                                    EagleDbSqlUnaryOperator *match);

#endif
