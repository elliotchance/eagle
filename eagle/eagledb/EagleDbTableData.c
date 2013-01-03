#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EagleDbTableData.h"
#include "EagleUtils.h"
#include "EagleMemory.h"

EagleDbTableData* EagleDbTableData_New(EagleDbTable *table)
{
    int i;
    EagleDbTableData *td = (EagleDbTableData*) malloc(sizeof(EagleDbTableData));
    
    td->table = table;
    td->usedProviders = table->usedColumns;
    td->providers = (EaglePageProvider**) calloc((size_t) td->usedProviders, sizeof(EaglePageProvider*));
    for(i = 0; i < td->usedProviders; ++i) {
        td->providers[i] = EaglePageProvider_CreateFromStream(table->columns[i]->type, 1000, table->columns[i]->name);
    }
    
    return td;
}

void EagleDbTableData_Delete(EagleDbTableData *td)
{
    int i;
    for(i = 0; i < td->usedProviders; ++i) {
        EaglePageProvider_Delete(td->providers[i]);
    }
    EagleMemory_Free(td->providers);
    EagleMemory_Free(td);
}

void EagleDbTableData_insert(EagleDbTableData *td, EagleDbTuple *tuple)
{
    int i;
    for(i = 0; i < td->table->usedColumns; ++i) {
        switch(td->providers[i]->type) {
                
            case EagleDataTypeUnknown:
                EagleUtils_Fatal("Unknown type.");
                
            case EagleDataTypeInteger:
                EaglePageProvider_add(td->providers[i], tuple->data[i]);
                break;
                
            case EagleDataTypeText:
                EaglePageProvider_add(td->providers[i], strdup(tuple->data[i]));
                break;
                
        }
    }
}
