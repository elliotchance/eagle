#include <stdlib.h>
#include <string.h>
#include "EagleDbColumn.h"
#include "EagleMemory.h"

EagleDbColumn* EagleDbColumn_New(char *name, EagleDataType type)
{
    EagleDbColumn *column = (EagleDbColumn*) EagleMemory_Allocate("EagleDbColumn_New.1", sizeof(EagleDbColumn));
    if(NULL == column) {
        return NULL;
    }
    
    column->name = (NULL == name ? NULL : strdup(name));
    column->type = type;
    
    return column;
}

void EagleDbColumn_Delete(EagleDbColumn *column)
{
    EagleMemory_Free(column->name);
    EagleMemory_Free(column);
}
