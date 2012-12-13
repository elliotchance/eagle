#include <stdlib.h>
#include <string.h>
#include "EagleDbColumn.h"

EagleDbColumn* EagleDbColumn_New(char *name, EagleDbColumnType type)
{
    EagleDbColumn *column = (EagleDbColumn*) malloc(sizeof(EagleDbColumn));
    
    column->name = strdup(name);
    column->type = type;
    
    return column;
}
