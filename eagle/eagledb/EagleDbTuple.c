#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EagleDbTuple.h"
#include "EagleData.h"
#include "EagleUtils.h"

EagleDbTuple* EagleDbTuple_New(EagleDbTable *table)
{
    EagleDbTuple *tuple = (EagleDbTuple*) malloc(sizeof(EagleDbTuple));
    
    tuple->table = table;
    tuple->data = (void**) calloc((size_t) table->usedColumns, sizeof(void*));
    
    return tuple;
}

void EagleDbTuple_Delete(EagleDbTuple *tuple)
{
    int i;
    
    for(i = 0; i < tuple->table->usedColumns; ++i) {
        free(tuple->data[i]);
    }
    free(tuple->data);
    
    free(tuple);
}

void EagleDbTuple_setInt(EagleDbTuple *tuple, int position, int value)
{
    if(tuple->table->columns[position]->type != EagleDataTypeInteger) {
        printf("WRONG TYPE!\n");
        return;
    }
    tuple->data[position] = EagleData_Int(value);
}

void EagleDbTuple_setText(EagleDbTuple *tuple, int position, char *value)
{
    if(tuple->table->columns[position]->type != EagleDataTypeText) {
        printf("WRONG TYPE!\n");
        return;
    }
    tuple->data[position] = strdup(value);
}

char* EagleDbTuple_toString(EagleDbTuple *tuple)
{
    char *desc = (char*) malloc(1024);
    int i;
    
    desc[0] = 0;
    strcat_safe(desc, "(");
    for(i = 0; i < tuple->table->usedColumns; ++i) {
        if(i > 0) {
            strcat_safe(desc, ",");
        }
        strcat_safe(desc, tuple->table->columns[i]->name);
        strcat_safe(desc, "=");
        
        switch(tuple->table->columns[i]->type) {
                
            case EagleDataTypeUnknown:
                sprintf(desc, "%s?", desc);
                break;
                
            case EagleDataTypeInteger:
                sprintf(desc, "%s%d", desc, *((int*) tuple->data[i]));
                break;
                
            case EagleDataTypeText:
                sprintf(desc, "%s\"%s\"", desc, (char*) tuple->data[i]);
                break;
                
        }
    }
    strcat_safe(desc, ")");
    
    return desc;
}
