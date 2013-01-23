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
 Create a new EagleDbSqlSelect.
 */
EagleDbSqlSelect* EagleDbSqlSelect_New(void);

void EagleDbSqlSelect_Delete(EagleDbSqlSelect *select, EagleBoolean recursive);

EaglePlan* EagleDbSqlSelect_parse(EagleDbSqlSelect *select, EagleDbInstance *db);

/**
 Return the number of expressions after the SELECT clause.
 */
int EagleDbSqlSelect_getFieldCount(EagleDbSqlSelect *select);

/**
 Returns the total amount of expressions for an entire SELECT statements (containing all its clauses)
 */
int EagleDbSqlSelect_getExpressionsCount(EagleDbSqlSelect *select);

char* EagleDbSqlSelect_toString(EagleDbSqlSelect *select);

#endif
