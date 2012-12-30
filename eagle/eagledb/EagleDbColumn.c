#include <stdlib.h>
#include <string.h>
#include "EagleDbColumn.h"

EagleDbColumn* EagleDbColumn_New(char *name, EagleDataType type)
{
    EagleDbColumn *column = (EagleDbColumn*) malloc(sizeof(EagleDbColumn));
    
    column->name = (NULL == name ? NULL : strdup(name));
    column->type = type;
    
    return column;
}

void EagleDbColumn_Delete(EagleDbColumn *column)
{
    free(column->name);
    free(column);
}
