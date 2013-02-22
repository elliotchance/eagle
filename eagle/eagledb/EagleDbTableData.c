#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EagleDbTableData.h"
#include "EagleUtils.h"
#include "EagleMemory.h"
#include "EagleLogger.h"

EagleDbTableData* EagleDbTableData_New(EagleDbTable *table, int pageSize)
{
    int i;
    EagleDbTableData *td;
    
    if(NULL == table) {
        return NULL;
    }
    
    td = (EagleDbTableData*) EagleMemory_Allocate("EagleDbTableData_New.1", sizeof(EagleDbTableData));
    if(NULL == td) {
        return NULL;
    }
    
    td->table = table;
    td->usedProviders = EagleDbTable_countColumns(table);
    if(td->usedProviders > 0) {
        td->providers = (EaglePageProvider**) EagleMemory_MultiAllocate("EagleDbTableData_New.2", sizeof(EaglePageProvider*), td->usedProviders);
        if(NULL == td->providers) {
            EagleMemory_Free(td);
            return NULL;
        }
    }
    else {
        td->providers = NULL;
    }
    
    for(i = 0; i < td->usedProviders; ++i) {
        EagleDbColumn *column = EagleDbTable_getColumn(table, i);
        td->providers[i] = EaglePageProvider_CreateFromStream(column->type, pageSize, column->name);
    }
    
    return td;
}

void EagleDbTableData_Delete(EagleDbTableData *td)
{
    if(NULL == td) {
        return;
    }
    
    {
        int i;
        for(i = 0; i < td->usedProviders; ++i) {
            EaglePageProvider_Delete(td->providers[i]);
        }
        EagleMemory_Free(td->providers);
        EagleMemory_Free(td);
    }
}

void EagleDbTableData_insert(EagleDbTableData *td, EagleDbTuple *tuple)
{
    int i;
    for(i = 0; i < EagleLinkedList_length(td->table->columns); ++i) {
        switch(td->providers[i]->type) {
                
            case EagleDataTypeUnknown:
                EagleLogger_Log(EagleLoggerSeverityError, "Unknown type.");
                return;
                
            case EagleDataTypeInteger:
                EaglePageProvider_add(td->providers[i], tuple->data[i]);
                break;
                
            case EagleDataTypeText:
                EaglePageProvider_add(td->providers[i], strdup(tuple->data[i]));
                break;
                
        }
    }
}
