#ifndef eagle_EagleDbInstance_h
#define eagle_EagleDbInstance_h

#include "EagleDbTableData.h"
#include "EaglePlan.h"

typedef enum {
    
    EagleDbSqlStatementTypeUnknown = 0,
    
    EagleDbSqlStatementTypeSelect = 1,
    
    EagleDbSqlStatementTypeCreateTable = 2
    
} EagleDbSqlStatementType;

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
