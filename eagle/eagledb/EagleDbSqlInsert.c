#include "EagleDbSqlInsert.h"
#include "EagleMemory.h"

EagleDbSqlInsert* EagleDbSqlInsert_New()
{
    EagleDbSqlInsert *insert = (EagleDbSqlInsert*) EagleMemory_Allocate("EagleDbSqlInsert_New.1", sizeof(EagleDbSqlInsert));
    if(NULL == insert) {
        return NULL;
    }
    
    insert->table = NULL;
    insert->names = NULL;
    insert->values = NULL;
    
    return insert;
}

void EagleDbSqlInsert_Delete(EagleDbSqlInsert *insert)
{
    if(NULL == insert) {
        return;
    }
    
    EagleLinkedList_Delete(insert->names);
    EagleLinkedList_Delete(insert->values);
    EagleMemory_Free(insert->table);
    EagleMemory_Free(insert);
}
