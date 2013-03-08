#ifndef eagle_EagleDbSqlFunctionExpression_h
#define eagle_EagleDbSqlFunctionExpression_h

#include "EagleDbSqlExpression.h"
#include "Eagle.h"

/**
 Expression type is EagleDbSqlExpressionTypeFunctionExpression.
 
 @see EagleDbSqlExpressionType
 */
typedef struct {
    
    /**
     Header.
     @see EagleDbSqlExpressionHeader
     */
    EagleDbSqlExpressionHeader;
    
    /**
     The name of the function.
     */
    EAGLE_ATTR_MANAGED char *name;
    
    /**
     The first argument.
     */
    EAGLE_ATTR_PROVIDED EagleDbSqlExpression *expr;
    
} EagleDbSqlFunctionExpression;

/**
 * Create a new EagleDbSqlFunctionExpression.
 * @param [in] name Function name.
 * @param [in] expr Expression.
 * @return A new instance.
 */
EagleDbSqlFunctionExpression* EagleDbSqlFunctionExpression_New(char *name, EagleDbSqlExpression *expr);

/**
 * Free a function expression.
 * @note It is safer to use EagleDbSqlExpression_Delete() since it will automatically use the correct Delete function.
 * @param [in] expr The instance.
 */
void EagleDbSqlFunctionExpression_Delete(EagleDbSqlFunctionExpression *expr);

/**
 * Free a function expression (recursively).
 * @note It is safer to use EagleDbSqlExpression_DeleteRecursive() since it will automatically use the correct Delete
 *       function.
 * @param [in] expr The instance.
 */
void EagleDbSqlFunctionExpression_DeleteRecursive(EagleDbSqlFunctionExpression *expr);

/**
 * Render a function expression into a string.
 * @param [in] expr The instance.
 * @return A new string representation of the expression.
 */
char* EagleDbSqlFunctionExpression_toString(EagleDbSqlFunctionExpression *expr);

#endif
