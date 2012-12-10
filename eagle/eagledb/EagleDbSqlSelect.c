#include <stdlib.h>
#include "EagleDbSqlSelect.h"

EagleDbSqlSelect* EagleDbSqlSelect_New(void)
{
    EagleDbSqlSelect *select = (EagleDbSqlSelect*) malloc(sizeof(EagleDbSqlSelect));
    select->tableName = NULL;
    return select;
}
