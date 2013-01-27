#ifndef eagle_EagleDbInstance_h
#define eagle_EagleDbInstance_h

#include "EagleDbSchema.h"
#include "EaglePlan.h"
#include "Eagle.h"
#include "EagleDbSqlStatementType.h"
#include "EagleDbSqlSelect.h"

extern EagleDbSqlStatementType yystatementtype;

typedef struct EagleDbInstance_ {
    
    /**
     Contains the pointers to the individual schemas. This is semi managed because the array that contains the pointers
     to the schemas is managed bu the schema objects themselves are managed externally (i.e. deleteing an
     EagleDbInstance) will not deleted the respective schemas.
     */
    EAGLE_ATTR_SEMI_MANAGED EagleDbSchema **schemas;
    
    /**
     The maximum possible schemas this instance can hold.
     */
    EAGLE_ATTR_NA int allocatedSchemas;
    
    /**
     The number of used (real) schemas connected to this instance.
     */
    EAGLE_ATTR_NA int usedSchemas;
    
    /**
     The page size to be provided to EaglePlan.
     */
    EAGLE_ATTR_NA int pageSize;
    
} EagleDbInstance;

EagleDbInstance* EagleDbInstance_New(int pageSize);

/**
 Free an instance.
 */
void EagleDbInstance_Delete(EagleDbInstance *db);

void EagleDbInstance_execute(EagleDbInstance *db, char *sql);

void EagleDbInstance_executeSelect(EagleDbInstance *db, EagleDbSqlSelect *select);

void EagleDbInstance_executeCreateTable(EagleDbInstance *db, EagleDbTable *table);

void EagleDbInstance_PrintResults(EaglePlan *plan);

EagleDbTableData* EagleDbInstance_getTable(EagleDbInstance *db, char *tableName);

EagleDbSchema* EagleDbInstance_getSchema(EagleDbInstance *db, char *schemaName);

void EagleDbInstance_addSchema(EagleDbInstance *db, EagleDbSchema *schema);

#endif
