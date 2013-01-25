#ifndef eagle_EagleDbSqlSelect_h
#define eagle_EagleDbSqlSelect_h

#include "EagleDbSqlExpression.h"
#include "EagleDbSqlValue.h"
#include "EagleDbInstance.h"
#include "Eagle.h"

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
     Represents the expression for the first column.
     */
    EAGLE_ATTR_PROVIDED EagleDbSqlExpression **selectExpressions;
    
    /**
     The number of allocated \c selectExpressions
     */
    EAGLE_ATTR_NA int allocatedSelectExpressions;
    
    /**
     The number of used \c selectExpressions
     */
    EAGLE_ATTR_NA int usedSelectExpressions;
    
} EagleDbSqlSelect;

/**
 * Create a new EagleDbSqlSelect.
 * @return A new instance.
 */
EagleDbSqlSelect* EagleDbSqlSelect_New(void);

/**
 * Free a SELECT object.
 * @param select The instance.
 * @param recursive If this is EagleTrue then all children will be freed as well, otherwise the
 *        memory management of every object in the AST will be your responsibility.
 */
void EagleDbSqlSelect_Delete(EagleDbSqlSelect *select, EagleBoolean recursive);

EaglePlan* EagleDbSqlSelect_parse(EagleDbSqlSelect *select, EagleDbInstance *db);

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
