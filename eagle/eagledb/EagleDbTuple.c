#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EagleDbTuple.h"
#include "EagleData.h"
#include "EagleUtils.h"
#include "EagleMemory.h"
#include "EagleLogger.h"
#include "EagleDbSqlValue.h"

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
    tuple->data = (void**) EagleMemory_MultiAllocate("EagleDbTuple_New.2", sizeof(void*), EagleDbTable_countColumns(table));
    if(NULL == tuple->data) {
        EagleDbTuple_Delete(tuple);
        return NULL;
    }
    
    return tuple;
}

void EagleDbTuple_Delete(EagleDbTuple *tuple)
{
    if(NULL == tuple) {
        return;
    }
    
    {
        int i;
        
        if(NULL != tuple->data) {
            for(i = 0; i < EagleDbTable_countColumns(tuple->table); ++i) {
                EagleMemory_Free(tuple->data[i]);
            }
            EagleMemory_Free(tuple->data);
        }
        
        EagleMemory_Free(tuple);
    }
}

void EagleDbTuple_setInt(EagleDbTuple *tuple, int position, EagleDataTypeIntegerType value)
{
    if(EagleDbTable_getColumn(tuple->table, position)->type != EagleDataTypeInteger) {
        EagleLogger_Log(EagleLoggerSeverityError, "Wrong type.");
        return;
    }
    tuple->data[position] = EagleData_Int(value);
}

void EagleDbTuple_setFloat(EagleDbTuple *tuple, int position, EagleDataTypeFloatType value)
{
    if(EagleDbTable_getColumn(tuple->table, position)->type != EagleDataTypeFloat) {
        EagleLogger_Log(EagleLoggerSeverityError, "Wrong type.");
        return;
    }
    tuple->data[position] = EagleData_Float(value);
}

void EagleDbTuple_setVarchar(EagleDbTuple *tuple, int position, EagleDataTypeVarcharType value)
{
    if(EagleDbTable_getColumn(tuple->table, position)->type != EagleDataTypeVarchar) {
        EagleLogger_Log(EagleLoggerSeverityError, "Wrong type.");
        return;
    }
    
    ((EagleDataTypeVarcharType*) tuple->data)[position] = strdup(value);
}

EagleBoolean EagleDbTuple_set(EagleDbTuple *tuple, int position, EagleDbSqlValue *v, EagleDataType columnType)
{
    EagleBoolean canCast;
    
    switch(columnType) {
            
        case EagleDataTypeInteger:
        {
            EagleDataTypeIntegerType value = EagleDbSqlValue_getInteger(v, &canCast);
            EagleDbTuple_setInt(tuple, position, value);
            break;
        }
            
        case EagleDataTypeFloat:
        {
            EagleDataTypeFloatType value = EagleDbSqlValue_getFloat(v, &canCast);
            EagleDbTuple_setFloat(tuple, position, value);
            break;
        }
            
        case EagleDataTypeVarchar:
        {
            EagleDataTypeVarcharType value = EagleDbSqlValue_getVarchar(v, &canCast);
            EagleDbTuple_setVarchar(tuple, position, value);
            break;
        }
            
        case EagleDataTypeUnknown:
        {
            canCast = EagleFalse;
            break;
        }
            
    }
    
    return canCast;
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
    for(i = 0; i < EagleDbTable_countColumns(tuple->table); ++i) {
        if(i > 0) {
            strcat_safe(desc, ",");
        }
        strcat_safe(desc, EagleDbTable_getColumn(tuple->table, i)->name);
        strcat_safe(desc, "=");
        
        switch(EagleDbTable_getColumn(tuple->table, i)->type) {
                
            case EagleDataTypeUnknown:
                sprintf(desc, "%s?", desc);
                break;
                
            case EagleDataTypeInteger:
                sprintf(desc, "%s%d", desc, *(((EagleDataTypeIntegerType**) tuple->data)[i]));
                break;
                
            case EagleDataTypeVarchar:
                sprintf(desc, "%s\"%s\"", desc, ((EagleDataTypeVarcharType*) tuple->data)[i]);
                break;
                
            case EagleDataTypeFloat:
                sprintf(desc, "%s%g", desc, *(((EagleDataTypeFloatType**) tuple->data)[i]));
                break;
                
        }
    }
    strcat_safe(desc, ")");
    
    return desc;
}
