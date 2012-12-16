#include <stdlib.h>
#include "EagleDbTableData.h"

EagleDbTableData* EagleDbTableData_New(EagleDbTable *table)
{
    int i;
    EagleDbTableData *td = (EagleDbTableData*) malloc(sizeof(EagleDbTableData));
    
    td->table = table;
    td->providers = (EaglePageProvider**) calloc((size_t) table->usedColumns, sizeof(EaglePageProvider*));
    for(i = 0; i < table->usedColumns; ++i) {
        td->providers[i] = EaglePageProvider_CreateFromIntStream(1000);
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
        EaglePageProvider_add(td->providers[i], tuple->data[i]);
    }
}
