#ifndef eagle_EagleDbSqlExpression_h
#define eagle_EagleDbSqlExpression_h

#include "EaglePlan.h"
#include "EagleDbSqlExpressionHeader.h"

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
    
    /**
     Header.
     @see EagleDbSqlExpressionHeader
     */
    EagleDbSqlExpressionHeader;
    
} EagleDbSqlExpression;

/**
 Return value when the expression can not be compiled.
 */
extern const int EagleDbSqlExpression_ERROR;

/**
 * Free an expression of any type.
 * @param [in] expr The instance.
 */
void EagleDbSqlExpression_Delete(EagleDbSqlExpression *expr);

void EagleDbSqlExpression_DeleteRecursive(EagleDbSqlExpression *expr);

/**
 * Compile a plan from one or more expressions.
 * @param [in] expressions An array of expressions.
 * @param [in] totalExpressions The total expressions in \p expressions
 * @param [in] whereClause If one of the expressions is the WHERE clause it needs to be highlighted by its index
 *        so that data generated by the expression can know which records need to be sent tot he result providers.
 * @param [in,out] plan The plan initialized to add the compiled expression into. This is marked as in and out
 *        parameter direction because you provide a skeleton plan to be written to, but you cannot provide NULL for
 *        this parameter.
 */
void EagleDbSqlExpression_CompilePlan(EagleDbSqlExpression **expressions, int totalExpressions, int whereClause, EaglePlan *plan);

/**
 * Render an expression (of any type) to a string.
 * @param [in] expr The instance.
 * @return A new string representation of the expression.
 */
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

/**
 Private function for EagleDbSqlExpression_CompilePlanIntoBuffer_().
 
 @param [in] expression See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 @param [in] destinationBuffer See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 @param [in] plan See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 @return See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 */
int EagleDbSqlExpression_CompilePlanIntoBuffer_Function_(EagleDbSqlExpression *expression, int *destinationBuffer, EaglePlan *plan);

/**
 Private function for EagleDbSqlExpression_CompilePlanIntoBuffer_().
 
 @param [in] expression See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 @param [in] destinationBuffer See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 @param [in] plan See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 @return See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 */
int EagleDbSqlExpression_CompilePlanIntoBuffer_Unary_(EagleDbSqlExpression *expression, int *destinationBuffer, EaglePlan *plan);

/**
 Private function for EagleDbSqlExpression_CompilePlanIntoBuffer_().
 
 @param [in] expression See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 @param [in] destinationBuffer See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 @param [in] plan See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 @return See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 */
int EagleDbSqlExpression_CompilePlanIntoBuffer_Binary_(const EagleDbSqlExpression *expression, int *destinationBuffer, EaglePlan *plan);

/**
 Private function for EagleDbSqlExpression_CompilePlanIntoBuffer_().
 
 @param [in] expression See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 @param [in] destinationBuffer See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 @param [in] plan See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 @return See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 */
int EagleDbSqlExpression_CompilePlanIntoBuffer_Cast_(const EagleDbSqlExpression *expression, int *destinationBuffer, EaglePlan *plan);

/**
 Private function for EagleDbSqlExpression_CompilePlanIntoBuffer_().
 
 @param [in] expression See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 @param [in] destinationBuffer See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 @param [in] plan See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 @return See EagleDbSqlExpression_CompilePlanIntoBuffer_().
 */
int EagleDbSqlExpression_CompilePlanIntoBuffer_Value_(EagleDbSqlExpression *expression, int *destinationBuffer, EaglePlan *plan);

#endif
