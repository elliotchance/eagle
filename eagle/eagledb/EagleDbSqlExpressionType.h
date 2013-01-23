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
    EagleDbSqlExpressionTypeSelect = 3
    
} EagleDbSqlExpressionType;

#endif
