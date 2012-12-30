#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EagleDbTableData.h"
#include "EagleUtils.h"

EagleDbTableData* EagleDbTableData_New(EagleDbTable *table)
{
    int i;
    EagleDbTableData *td = (EagleDbTableData*) malloc(sizeof(EagleDbTableData));
    
    td->table = table;
    td->providers = (EaglePageProvider**) calloc((size_t) table->usedColumns, sizeof(EaglePageProvider*));
    for(i = 0; i < table->usedColumns; ++i) {
        td->providers[i] = EaglePageProvider_CreateFromStream(table->columns[i]->type, 1000, table->columns[i]->name);
    }
    
    return td;
}

void EagleDbTableData_Delete(EagleDbTableData *td)
{
    int i;
    for(i = 0; i < td->table->usedColumns; ++i) {
        EaglePageProvider_Delete(td->providers[i]);
    }
    free(td->providers);
    free(td);
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
