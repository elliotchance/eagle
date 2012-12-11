#ifndef eagle_EagleDbSqlSelect_h
#define eagle_EagleDbSqlSelect_h

#include "EagleDbSqlExpression.h"
#include "EagleDbSqlValue.h"

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
    EagleDbSqlValue *whereExpression;
    
} EagleDbSqlSelect;

EagleDbSqlSelect* EagleDbSqlSelect_New(void);

#endif
