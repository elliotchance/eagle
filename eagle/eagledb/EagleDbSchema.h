#ifndef eagle_EagleDbSchema_h
#define eagle_EagleDbSchema_h

#include "EagleDbTableData.h"
#include "Eagle.h"
#include "EagleLinkedList.h"

/**
 The name of the default schema.
 */
extern const char *EagleDbSchema_DefaultSchemaName;

/**
 The name of the information_schema.
 */
extern const char *EagleDbSchema_InformationSchemaName;

/**
 A database schema definition.
 */
typedef struct {
    
    /**
     This is semi managed because the linked list that contains the pointers to the tables is managed internally but the
     table objects themselves are managed externally (i.e. deleteing a EagleDbSchema will not delete the tables it
     contains)
     */
    EAGLE_ATTR_SEMI_MANAGED EagleLinkedList *tables;
    
    /**
     Schema name.
     */
    EAGLE_ATTR_MANAGED char *name;
    
} EagleDbSchema;

EagleDbSchema* EagleDbSchema_New(char *name);

void EagleDbSchema_Delete(EagleDbSchema *schema);

EagleBoolean EagleDbSchema_addTable(EagleDbSchema *schema, EagleDbTableData *td);

EagleDbTableData* EagleDbSchema_getTable(EagleDbSchema *schema, const char *tableName);

#endif
