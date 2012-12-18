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
    EagleDbSqlExpressionOperatorEquals = 2
    
} EagleDbSqlExpressionOperator;

#endif
