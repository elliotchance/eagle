#ifndef eagle_EagleDbSqlExpressionType_h
#define eagle_EagleDbSqlExpressionType_h

/**
 The type of expression.
 */
typedef enum {
    
    /** Real type is EagleDbSqlValue */
    EagleDbSqlExpressionTypeValue = 1,
    
    /** Real type is EagleDbSqlBinaryExpression */
    EagleDbSqlExpressionTypeBinaryExpression = 2,
    
    /** Real type is EagleDbSqlSelect */
    EagleDbSqlExpressionTypeSelect = 3,
    
    /** Real type is EagleDbSqlUnaryExpression */
    EagleDbSqlExpressionTypeUnaryExpression = 4,
    
    /** Real type is EagleDbSqlFunctionExpression */
    EagleDbSqlExpressionTypeFunctionExpression = 5,
    
    /** Real type is EagleDbSqlCastExpression */
    EagleDbSqlExpressionTypeCastExpression = 6
    
} EagleDbSqlExpressionType;

#endif
