#ifndef eagle_EagleDbSqlUnaryExpressionOperator_h
#define eagle_EagleDbSqlUnaryExpressionOperator_h

/**
 Unary expression operators.
 */
typedef enum {
    
    /**
     Negate operator.
     */
    EagleDbSqlUnaryExpressionOperatorNegate = 1,
    
    /**
     Logical NOT operator.
     */
    EagleDbSqlUnaryExpressionOperatorNot = 2
    
} EagleDbSqlUnaryExpressionOperator;

/**
 * Get the string representation of the operator.
 * @param [in] op The operator.
 * @return A new string. You must free this yourself.
 */
char* EagleDbSqlUnaryExpressionOperator_toString(EagleDbSqlUnaryExpressionOperator op);

#endif
