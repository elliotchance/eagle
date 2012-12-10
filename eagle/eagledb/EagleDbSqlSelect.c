#include <stdlib.h>
#include "EagleDbSqlSelect.h"

EagleDbSqlSelect* EagleDbSqlSelect_New(void)
{
    EagleDbSqlSelect *select = (EagleDbSqlSelect*) malloc(sizeof(EagleDbSqlSelect));
    select->tableName = NULL;
    select->whereExpression = NULL;
    return select;
}
