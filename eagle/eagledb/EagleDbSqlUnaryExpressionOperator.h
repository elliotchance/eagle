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
    EagleDbSqlUnaryExpressionOperatorNot = 2,
    
    /**
     Grouping () operator.
     */
    EagleDbSqlUnaryExpressionOperatorGrouping = 3
    
} EagleDbSqlUnaryExpressionOperator;

/**
 * Get the string representation of the operator. The \p before and \p after will be allocated automatically but you
 * must free both of them (even if the value is blank)
 * @param [in] op The operator.
 * @param [out] before The string to put before the expression.
 * @param [out] after The string to put before the expression.
 */
void EagleDbSqlUnaryExpressionOperator_toString(EagleDbSqlUnaryExpressionOperator op, char **before, char **after);

#endif
