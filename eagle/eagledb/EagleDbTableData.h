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

/**
 * Create a new table data instance. This structure extends the definition of the table to
 * encapsulate the actual column data for the table.
 * @param [in] table The table instance.
 * @param [in] pageSize The page size for the providers (when data is read from the table).
 * @return A new instance.
 */
EagleDbTableData* EagleDbTableData_New(EagleDbTable *table, int pageSize);

/**
 * Free a table data. This will not free the table definition.
 * @param [in] td The instance.
 */
void EagleDbTableData_Delete(EagleDbTableData *td);

/**
 * INSERT a new tuple (record) onto the table data.
 * @param [in] td The table data to receive the new record.
 * @param [in] tuple The tuple to append.
 */
void EagleDbTableData_insert(EagleDbTableData *td, EagleDbTuple *tuple);

#endif
