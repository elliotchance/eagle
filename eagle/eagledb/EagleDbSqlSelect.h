#ifndef eagle_EagleDbSqlSelect_h
#define eagle_EagleDbSqlSelect_h

#include "EagleDbSqlExpression.h"
#include "EagleDbSqlValue.h"
#include "EagleDbInstance.h"

/**
 SQL SELECT expression.
 
 Expression type is EagleDbSqlExpressionTypeSelect.
 */
typedef struct {
    
    EagleDbSqlExpressionHeader;
    
    /**
     The name of the table in the FROM clause.
     */
    char *tableName;
    
    /**
     An optional WHERE clause. This may be NULL indicating there was no WHERE specified.
     */
    EagleDbSqlExpression *whereExpression;
    
    /**
     Represents the expression for the first column.
     */
    EagleDbSqlExpression **selectExpressions;
    
    /**
     The number of allocated \c selectExpressions
     */
    int allocatedSelectExpressions;
    
    /**
     The number of used \c selectExpressions
     */
    int usedSelectExpressions;
    
} EagleDbSqlSelect;

/**
 Create a new EagleDbSqlSelect.
 */
EagleDbSqlSelect* EagleDbSqlSelect_New(void);

void EagleDbSqlSelect_Delete(EagleDbSqlSelect *select);

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
