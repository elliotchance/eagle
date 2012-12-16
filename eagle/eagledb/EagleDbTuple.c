#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EagleDbTuple.h"
#include "EagleData.h"

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
    tuple->data[position] = EagleData_Int(value);
}

char* EagleDbTuple_toString(EagleDbTuple *tuple)
{
    char *desc = (char*) malloc(1024);
    int i;
    
    desc[0] = 0;
    strcat(desc, "(");
    for(i = 0; i < tuple->table->usedColumns; ++i) {
        if(i > 0) {
            strcat(desc, ",");
        }
        strcat(desc, tuple->table->columns[i]->name);
        strcat(desc, "=");
        
        switch(tuple->table->columns[i]->type) {
            case EagleDbColumnTypeInteger:
                sprintf(desc, "%s%d", desc, *((int*) tuple->data[i]));
                break;
        }
    }
    strcat(desc, ")");
    
    return desc;
}
