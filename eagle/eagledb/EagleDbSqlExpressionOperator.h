#ifndef eagle_EagleDbSqlExpressionOperator_h
#define eagle_EagleDbSqlExpressionOperator_h

/**
 Expression operators.
 
 This includes pre and post unary operators as well as binary operators.
 */
typedef enum {
    
    /**
     Addition operator.
     */
    EagleDbSqlExpressionOperatorPlus = 1,
    
    /**
     Equals operator.
     */
    EagleDbSqlExpressionOperatorEquals = 2,
    
    /**
     Modulus (remainder) operator.
     */
    EagleDbSqlExpressionOperatorModulus = 3,
    
    /**
     Multiplication operator.
     */
    EagleDbSqlExpressionOperatorMultiply = 4,
    
    /**
     Not equals operator.
     */
    EagleDbSqlExpressionOperatorNotEquals = 5,
    
    /**
     Greater than operator.
     */
    EagleDbSqlExpressionOperatorGreaterThan = 6,
    
    /**
     Less than operator.
     */
    EagleDbSqlExpressionOperatorLessThan = 7,
    
    /**
     Greater than or equal to operator.
     */
    EagleDbSqlExpressionOperatorGreaterThanEqual = 8,
    
    /**
     Greater than or equal to operator.
     */
    EagleDbSqlExpressionOperatorLessThanEqual = 9
    
} EagleDbSqlExpressionOperator;

/**
 * Get the string representation of the operator.
 * @param [in] op The operator.
 * @return A new string. You must free this yourself.
 */
char* EagleDbSqlExpressionOperator_toString(EagleDbSqlExpressionOperator op);

#endif
