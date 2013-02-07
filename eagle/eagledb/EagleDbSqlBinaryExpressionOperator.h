#ifndef eagle_EagleDbSqlBinaryExpressionOperator_h
#define eagle_EagleDbSqlBinaryExpressionOperator_h

/**
 Bianry expression operators.
 */
typedef enum {
    
    /**
     Addition operator.
     */
    EagleDbSqlBinaryExpressionOperatorPlus = 1,
    
    /**
     Equals operator.
     */
    EagleDbSqlBinaryExpressionOperatorEquals = 2,
    
    /**
     Modulus (remainder) operator.
     */
    EagleDbSqlBinaryExpressionOperatorModulus = 3,
    
    /**
     Multiplication operator.
     */
    EagleDbSqlBinaryExpressionOperatorMultiply = 4,
    
    /**
     Not equals operator.
     */
    EagleDbSqlBinaryExpressionOperatorNotEquals = 5,
    
    /**
     Greater than operator.
     */
    EagleDbSqlBinaryExpressionOperatorGreaterThan = 6,
    
    /**
     Less than operator.
     */
    EagleDbSqlBinaryExpressionOperatorLessThan = 7,
    
    /**
     Greater than or equal to operator.
     */
    EagleDbSqlBinaryExpressionOperatorGreaterThanEqual = 8,
    
    /**
     Greater than or equal to operator.
     */
    EagleDbSqlBinaryExpressionOperatorLessThanEqual = 9,
    
    /**
     Subtraction (or negate) operator.
     */
    EagleDbSqlBinaryExpressionOperatorMinus = 10,
    
    /**
     Division operator.
     */
    EagleDbSqlBinaryExpressionOperatorDivide = 11,
    
    /**
     Logical OR operator.
     */
    EagleDbSqlBinaryExpressionOperatorOr = 12,
    
    /**
     Logical AND operator.
     */
    EagleDbSqlBinaryExpressionOperatorAnd = 13
    
} EagleDbSqlBinaryExpressionOperator;

/**
 * Get the string representation of the operator.
 * @param [in] op The operator.
 * @return A new string. You must free this yourself.
 */
char* EagleDbSqlBinaryExpressionOperator_toString(EagleDbSqlBinaryExpressionOperator op);

#endif
