#include <stdlib.h>
#include "EagleDbSqlSelect.h"

/**
 Create a new EagleDbSqlSelect.
 */
EagleDbSqlSelect* EagleDbSqlSelect_New(void)
{
    EagleDbSqlSelect *select = (EagleDbSqlSelect*) malloc(sizeof(EagleDbSqlSelect));
    
    select->expressionType = EagleDbSqlExpressionTypeSelect;
    select->tableName = NULL;
    select->whereExpression = NULL;
    
    return select;
}

void EagleDbSqlSelect_Delete(EagleDbSqlSelect *select)
{
    free(select->tableName);
    EagleDbSqlExpression_Delete(select->whereExpression);
    free(select);
}
