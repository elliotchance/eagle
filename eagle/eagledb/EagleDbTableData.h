#ifndef eagle_EagleDbTableData_h
#define eagle_EagleDbTableData_h

#include "EagleDbTable.h"
#include "EaglePageProvider.h"
#include "EagleDbTuple.h"
#include "Eagle.h"

/**
 Provides data for a table.
 */
typedef struct {
    
    /**
     Table definition.
     */
    EAGLE_ATTR_PROVIDED EagleDbTable *table;
    
    /**
     Providers for each column.
     */
    EAGLE_ATTR_MANAGED EaglePageProvider **providers;
    
    /**
     The number of used \c providers
     */
    EAGLE_ATTR_NA int usedProviders;
    
} EagleDbTableData;

EagleDbTableData* EagleDbTableData_New(EagleDbTable *table, int pageSize);

void EagleDbTableData_Delete(EagleDbTableData *td);

void EagleDbTableData_insert(EagleDbTableData *td, EagleDbTuple *tuple);

#endif
