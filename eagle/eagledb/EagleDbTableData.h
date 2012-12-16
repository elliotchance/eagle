#ifndef eagle_EagleDbTableData_h
#define eagle_EagleDbTableData_h

#include "EagleDbTable.h"
#include "EaglePageProvider.h"
#include "EagleDbTuple.h"

/**
 Provides data for a table.
 */
typedef struct {
    
    /**
     Table definition.
     */
    EagleDbTable *table;
    
    /**
     Providers for each column.
     */
    EaglePageProvider **providers;
    
} EagleDbTableData;

EagleDbTableData* EagleDbTableData_New(EagleDbTable *table);
void EagleDbTableData_Delete(EagleDbTableData *td);

void EagleDbTableData_insert(EagleDbTableData *td, EagleDbTuple *tuple);

#endif
