#ifndef eagle_EagleDbSchema_h
#define eagle_EagleDbSchema_h

#include "EagleDbTableData.h"
#include "Eagle.h"

/**
 The name of the default schema.
 */
extern const char *EagleDbSchema_DefaultSchemaName;

/**
 A database schema definition.
 */
typedef struct {
    
    /**
     The number of allocated tables.
     */
    EAGLE_ATTR_NA int allocatedTables;
    
    /**
     The number of used tables.
     */
    EAGLE_ATTR_NA int usedTables;
    
    /**
     This is semi managed because the array that contains the pointers to the tables is managed internally but the table
     objects themselves are managed externally (i.e. deleteing a EagleDbSchema will not delete the tables it contains)
     */
    EAGLE_ATTR_SEMI_MANAGED EagleDbTableData **tables;
    
    /**
     Schema name.
     */
    EAGLE_ATTR_MANAGED char *name;
    
} EagleDbSchema;

EagleDbSchema* EagleDbSchema_New(char *name);

void EagleDbSchema_Delete(EagleDbSchema *schema);

void EagleDbSchema_addTable(EagleDbSchema *schema, EagleDbTableData *td);

#endif
