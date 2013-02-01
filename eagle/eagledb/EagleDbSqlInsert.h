#ifndef eagle_EagleDbSqlInsert_h
#define eagle_EagleDbSqlInsert_h

#include "Eagle.h"
#include "EagleLinkedList.h"

/**
 Represents an INSERT statement.
 */
typedef struct {
    
    /**
     The name of the table to insert into.
     */
    EAGLE_ATTR_MANAGED char *table;
    
    /**
     A linked list of C strings for columns names.
     */
    EAGLE_ATTR_MANAGED EagleLinkedList *names;
    
    /**
     A linked list of EagleDbSqlExpression's for VALUES.
     */
    EAGLE_ATTR_MANAGED EagleLinkedList *values;
    
} EagleDbSqlInsert;

/**
 Create a new INSERT.
 @return New instance.
 */
EagleDbSqlInsert* EagleDbSqlInsert_New(void);

/**
 Delete an INSERT object.
 @param [in] insert The object.
 */
void EagleDbSqlInsert_Delete(EagleDbSqlInsert *insert);

#endif
