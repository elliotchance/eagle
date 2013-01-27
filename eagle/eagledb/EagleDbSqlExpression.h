#ifndef eagle_EagleDbSqlExpression_h
#define eagle_EagleDbSqlExpression_h

#include "EaglePlan.h"
#include "EagleDbSqlExpressionType.h"

#define EagleDbSqlExpressionHeader EagleDbSqlExpressionType expressionType

extern const int EagleDbSqlExpression_ERROR;

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

/**
 Return value when the expression can not be compiled.
 */
extern const int EagleDbSqlExpression_ERROR;

void EagleDbSqlExpression_Delete(EagleDbSqlExpression *expr);

void EagleDbSqlExpression_DeleteRecursive(EagleDbSqlExpression *expr);

void EagleDbSqlExpression_CompilePlan(EagleDbSqlExpression **expressions, int totalExpressions, int whereClause, EaglePlan *plan);

char* EagleDbSqlExpression_toString(EagleDbSqlExpression *expr);

/**
 Recursive function to compile an expression into a plan.
 
 @param [in] expression The expression to compile.
 @param [in] destinationBuffer This number is incremented throughout the recursion. When you first call this function
 the \p destinationBuffer will be the position of the first buffer allowed to be writen to. You will likely want
 to set this to 1 so that you keep buffer 0 available. Note that this is not where there final result will be
 put, it is only the start ID of which buffers can be written to - the function will return an int which is the
 buffer ID with the real result. You may need to copy that buffer into buffer 0.
 @param [in] plan The plan the operations will be compiled into.
 @return The buffer ID that contains the real result.
 */
int EagleDbSqlExpression_CompilePlanIntoBuffer_(EagleDbSqlExpression *expression, int *destinationBuffer, EaglePlan *plan);

#endif
