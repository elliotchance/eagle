#ifndef eagle_EagleDbInstance_h
#define eagle_EagleDbInstance_h

#include "EagleDbSchema.h"
#include "EaglePlan.h"
#include "Eagle.h"
#include "EagleDbSqlStatementType.h"
#include "EagleDbSqlSelect.h"
#include "EagleDbSqlInsert.h"
#include "EagleLoggerEvent.h"

struct EagleDbParser;

/**
 The database instance is a wrapper for a EagleInstance.
 */
typedef struct EagleDbInstance {
    
    /**
     Contains the pointers to the individual schemas. The schema objects themselves are managed externally (i.e.
     deleteing an EagleDbInstance) will not deleted the respective schemas.
     */
    EAGLE_ATTR_SEMI_MANAGED EagleLinkedList *schemas;
    
    /**
     The page size to be provided to EaglePlan.
     */
    EAGLE_ATTR_NA int pageSize;
    
    /**
     The number of CPU cores.
     */
    EAGLE_ATTR_NA int cores;
    
} EagleDbInstance;

/**
 Create a new database instance.
 @param [in] pageSize The default page size to use for EaglePlan.
 @param [in] cores The number of CPU cores.
 @return A new instance.
 */
EagleDbInstance* EagleDbInstance_New(int pageSize, int cores);

/**
 Free an instance.
 @param [in] db The database instance.
 */
void EagleDbInstance_Delete(EagleDbInstance *db);

/**
 Free an instance and ALL of the schemas and tables.
 @param [in] db The database instance.
 */
void EagleDbInstance_DeleteAll(EagleDbInstance *db);

/**
 Execute an arbitrary SQL statement.
 @param [in] db The database instance.
 @param [in] sql The SQL statement.
 @param [out] error If an error occurs it will be sent back through this parameter.
 */
EagleBoolean EagleDbInstance_execute(EagleDbInstance *db, const char *sql, EagleLoggerEvent **error);

/**
 Mostly for internal use. See EagleDbInstance_execute().
 @param [in] db The database instance.
 @param [in] p Parser instance.
 @param [out] error If an error occurs it will be sent back through this parameter.
 @return Success status.
 */
EagleBoolean EagleDbInstance_executeParser(EagleDbInstance *db, struct EagleDbParser *p, EagleLoggerEvent **error);

/**
 Execute a SELECT statement.
 @param [in] db The database instance.
 @param [in] select The SELECT statement (compiled).
 @param [out] error If an error occurs it will be sent back through this parameter.
 */
EagleBoolean EagleDbInstance_executeSelect(EagleDbInstance *db, EagleDbSqlSelect *select, EagleLoggerEvent **error);

/**
 Execute a CREATE TABLE statement.
 @param [in] db The database instance.
 @param [in] table The table definition to create.
 @param [out] error If an error occurs it will be sent back through this parameter.
 */
EagleBoolean EagleDbInstance_executeCreateTable(EagleDbInstance *db, EagleDbTable *table, EagleLoggerEvent **error);

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
EagleDbSchema* EagleDbInstance_getSchema(EagleDbInstance *db, const char *schemaName);

/**
 Add a schema to the instance.
 @param [in] db The database instance.
 @param [in] schema The schema.
 @return EagleTrue if it is successful.
 */
EagleBoolean EagleDbInstance_addSchema(EagleDbInstance *db, EagleDbSchema *schema);

/**
 Execute an INSERT statement.
 @param [in] db Database instance.
 @param [in] insert INSERT statement.
 @param [out] error If an error occurs it will be sent back through this parameter.
 @return EagleTrue on success.
 */
EagleBoolean EagleDbInstance_executeInsert(EagleDbInstance *db, EagleDbSqlInsert *insert, EagleLoggerEvent **error);

#endif
