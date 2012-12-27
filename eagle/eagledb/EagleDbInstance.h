#ifndef eagle_EagleDbInstance_h
#define eagle_EagleDbInstance_h

#include "EagleDbTableData.h"
#include "EaglePlan.h"

typedef enum {
    
    /**
     Indicated a blank SQL statement. This may or may not be an error depending on the context.
     */
    EagleDbSqlStatementTypeNone = 0,
    
    /**
     SELECT statement.
     @see EagleDbSqlSelect
     */
    EagleDbSqlStatementTypeSelect = 1,
    
    /**
     CREATE TABLE statement.
     @see EagleDbTable
     */
    EagleDbSqlStatementTypeCreateTable = 2
    
} EagleDbSqlStatementType;

extern EagleDbSqlStatementType yystatementtype;

typedef struct {
    
    EagleDbTableData *td;
    
} EagleDbInstance;

EagleDbInstance* EagleDbInstance_New(void);
void EagleDbInstance_Delete(EagleDbInstance *db);
void EagleDbInstance_execute(EagleDbInstance *db, char *sql);
void EagleDbInstance_executeSelect(EagleDbInstance *db);
void EagleDbInstance_executeCreateTable(EagleDbInstance *db);
void EagleDbInstance_PrintResults(EaglePlan *plan);

#endif
