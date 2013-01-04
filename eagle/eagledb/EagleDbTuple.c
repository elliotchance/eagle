#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EagleDbTuple.h"
#include "EagleData.h"
#include "EagleUtils.h"
#include "EagleMemory.h"

EagleDbTuple* EagleDbTuple_New(EagleDbTable *table)
{
    EagleDbTuple *tuple;
    
    if(NULL == table) {
        return NULL;
    }
    
    tuple = (EagleDbTuple*) EagleMemory_Allocate("EagleDbTuple_New.1", sizeof(EagleDbTuple));
    if(NULL == tuple) {
        return NULL;
    }
    
    tuple->table = table;
    tuple->data = (void**) EagleMemory_MultiAllocate("EagleDbTuple_New.2", sizeof(void*), table->usedColumns);
    if(NULL == tuple->data) {
        EagleDbTuple_Delete(tuple);
        return NULL;
    }
    
    return tuple;
}

void EagleDbTuple_Delete(EagleDbTuple *tuple)
{
    int i;
    
    if(NULL != tuple->data) {
        for(i = 0; i < tuple->table->usedColumns; ++i) {
            EagleMemory_Free(tuple->data[i]);
        }
        EagleMemory_Free(tuple->data);
    }
    
    EagleMemory_Free(tuple);
}

void EagleDbTuple_setInt(EagleDbTuple *tuple, int position, int value)
{
    if(tuple->table->columns[position]->type != EagleDataTypeInteger) {
        EagleUtils_Fatal("Wrong type!");
    }
    tuple->data[position] = EagleData_Int(value);
}

void EagleDbTuple_setText(EagleDbTuple *tuple, int position, char *value)
{
    if(tuple->table->columns[position]->type != EagleDataTypeText) {
        EagleUtils_Fatal("Wrong type!");
    }
    
    ((char**) tuple->data)[position] = strdup(value);
}

char* EagleDbTuple_toString(EagleDbTuple *tuple)
{
    char *desc = (char*) EagleMemory_Allocate("EagleDbTuple_toString.1", 1024);
    int i;
    
    if(NULL == desc) {
        return NULL;
    }
    
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
                sprintf(desc, "%s%d", desc, *(((int**) tuple->data)[i]));
                break;
                
            case EagleDataTypeText:
                sprintf(desc, "%s\"%s\"", desc, ((char**) tuple->data)[i]);
                break;
                
        }
    }
    strcat_safe(desc, ")");
    
    return desc;
}
