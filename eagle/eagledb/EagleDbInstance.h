#ifndef eagle_EagleDbInstance_h
#define eagle_EagleDbInstance_h

#include "EagleDbSchema.h"
#include "EaglePlan.h"
#include "Eagle.h"
#include "EagleDbSqlStatementType.h"
#include "EagleDbSqlSelect.h"

extern EagleDbSqlStatementType yystatementtype;

/**
 The database instance is a wrapper for a EagleInstance.
 */
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

/**
 Create a new database instance.
 @param [in] pageSize The default page size to use for EaglePlan.
 @return A new instance.
 */
EagleDbInstance* EagleDbInstance_New(int pageSize);

/**
 Free an instance.
 @param [in] db The database instance.
 */
void EagleDbInstance_Delete(EagleDbInstance *db);

/**
 Execute an arbitrary SQL statement.
 @param [in] db The database instance.
 @param [in] sql The SQL statement.
 */
void EagleDbInstance_execute(EagleDbInstance *db, char *sql);

/**
 Execute a SELECT statement.
 @param [in] db The database instance.
 @param [in] select The SELECT statement (compiled).
 */
void EagleDbInstance_executeSelect(EagleDbInstance *db, EagleDbSqlSelect *select);

/**
 Execute a CREATE TABLE statement.
 @param [in] db The database instance.
 @param [in] table The table definition to create.
 */
void EagleDbInstance_executeCreateTable(EagleDbInstance *db, EagleDbTable *table);

/**
 Print a result set (fancy).
 @param [in] plan The plan contains a pointer to the result set once it is executed.
 */
void EagleDbInstance_PrintResults(EaglePlan *plan);

/**
 Get a table by name. This will only search the EagleDbSchema_DefaultSchemaName schema. It does not support schema name
 with a dot, yet.
 @param [in] db The database instance.
 @param [in] tableName The raw table name.
 @return NULL if the table can not be found.
 */
EagleDbTableData* EagleDbInstance_getTable(EagleDbInstance *db, char *tableName);

/**
 Get a schema by name.
 @param [in] db The database instance.
 @param [in] schemaName The name of the schema.
 @return NULL if the schema can not be found.
 */
EagleDbSchema* EagleDbInstance_getSchema(EagleDbInstance *db, char *schemaName);

/**
 Add a schema to the instance.
 @param [in] db The database instance.
 @param [in] schema The schema.
 */
void EagleDbInstance_addSchema(EagleDbInstance *db, EagleDbSchema *schema);

#endif
