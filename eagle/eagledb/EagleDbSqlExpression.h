#ifndef eagle_EagleDbSqlExpression_h
#define eagle_EagleDbSqlExpression_h

#include "EaglePlan.h"

#define EagleDbSqlExpressionHeader EagleDbSqlExpressionType expressionType

extern const int EagleDbSqlExpression_ERROR;

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

/**
 Dummy type for other expression structures.
 
 This structure allows the passing of other expression structures without needing to know the real type until runtime.
 
 Each subexpression type MUST include EagleDbSqlExpressionHeader as the first entry so that structure can be identified.
 For example:
 
 @code
 EagleDbSqlExpression *expr = ...
 if(expr->type == EagleDbSqlExpressionTypeValue) {
     EagleDbSqlValue *value = (EagleDbSqlValue*) expr;
     if(value->type == EagleDbSqlValueTypeInteger) {
         printf("Integer value = %s\n", value->value.intValue);
     }
     else {
         printf("Error: expected EagleDbSqlValue to be an integer.\n");
     }
 }
 else {
     printf("Error: expected expr to be a Value.\n");
 }
 @endcode
 */
typedef struct {
    
    EagleDbSqlExpressionHeader;
    
} EagleDbSqlExpression;

void EagleDbSqlExpression_Delete(EagleDbSqlExpression *expr);
void EagleDbSqlExpression_CompilePlan(EagleDbSqlExpression **expressions, int totalExpressions, int whereClause, EaglePlan *plan);

/* private functions */
int EagleDbSqlExpression_CompilePlanIntoBuffer_(EagleDbSqlExpression *expression, int *destinationBuffer, EaglePlan *plan);

#endif
