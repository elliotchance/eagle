#ifndef eagle_EagleDbSqlSelect_h
#define eagle_EagleDbSqlSelect_h

#include "EagleDbSqlExpression.h"
#include "EagleDbSqlValue.h"
#include "Eagle.h"
#include "EagleLinkedList.h"

struct EagleDbInstance_;

/**
 SQL SELECT expression.
 
 Expression type is EagleDbSqlExpressionTypeSelect.
 */
typedef struct {
    
    /**
     @see EagleDbSqlExpressionHeader
     */
    EagleDbSqlExpressionHeader;
    
    /**
     The name of the table in the FROM clause.
     */
    EAGLE_ATTR_MANAGED char *tableName;
    
    /**
     An optional WHERE clause. This may be NULL indicating there was no WHERE specified.
     */
    EAGLE_ATTR_PROVIDED EagleDbSqlExpression *whereExpression;
    
    /**
     Represents the expressions for the columns.
     */
    EAGLE_ATTR_PROVIDED EagleLinkedList *selectExpressions;
    
} EagleDbSqlSelect;

/**
 * Create a new EagleDbSqlSelect.
 * @return A new instance.
 */
EagleDbSqlSelect* EagleDbSqlSelect_New(void);

/**
 * Free a SELECT object.
 * @param select The instance.
 */
void EagleDbSqlSelect_Delete(EagleDbSqlSelect *select);

void EagleDbSqlSelect_DeleteRecursive(EagleDbSqlSelect *select);

EaglePlan* EagleDbSqlSelect_parse(EagleDbSqlSelect *select, struct EagleDbInstance_ *db);

/**
 * Return the number of expressions after the SELECT clause.
 * @param select The instance.
 * @return See description.
 */
int EagleDbSqlSelect_getFieldCount(EagleDbSqlSelect *select);

/**
 * Returns the total amount of expressions for an entire SELECT statements (containing all its
 * clauses)
 * @param select The instance.
 * @return See description.
 */
int EagleDbSqlSelect_getExpressionsCount(EagleDbSqlSelect *select);

/**
 * Render the AST into a SELECT statement. The SELECT generated will be neat and will not contain
 * the same formatting as the original SELECT that was parsed - however it will be valid SQL and
 * re-parsing the value returned from this function will generate the same AST tree.
 * @param select The instance.
 * @return String SQL.
 */
char* EagleDbSqlSelect_toString(EagleDbSqlSelect *select);

#endif
