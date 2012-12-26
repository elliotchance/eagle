#include <stdlib.h>
#include <stdio.h>

#include "EagleDbInstance.h"
#include "EagleDbTable.h"
#include "EagleDbTuple.h"

EagleDbInstance* EagleDbInstance_New(void)
{
    int i;
    EagleDbInstance *db = (EagleDbInstance*) malloc(sizeof(EagleDbInstance));
    
    /* create a virtual table */
    EagleDbTable *table = EagleDbTable_New("t");
    EagleDbTable_addColumn(table, EagleDbColumn_New("col1", EagleDbColumnTypeInteger));
    EagleDbTable_addColumn(table, EagleDbColumn_New("col2", EagleDbColumnTypeInteger));
    
    /* put some data in it */
    db->td = EagleDbTableData_New(table);
    
    for(i = 0; i < 10; ++i) {
        /* create a record */
        EagleDbTuple *tuple = EagleDbTuple_New(table);
        EagleDbTuple_setInt(tuple, 0, i);
        EagleDbTuple_setInt(tuple, 1, i * 2);
        
        /* put record in */
        EagleDbTableData_insert(db->td, tuple);
    }
    
    return db;
}

void EagleDbInstance_execute(EagleDbInstance *db, char *sql)
{
    printf("'%s'\n", sql);
}

void EagleDbInstance_Delete(EagleDbInstance *db)
{
    free(db);
}
