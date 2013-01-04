#ifndef eagle_EagleDbSchema_h
#define eagle_EagleDbSchema_h

#include "EagleDbTableData.h"

typedef struct {
    
    int allocatedTables;
    
    int usedTables;
    
    EagleDbTableData **tables;
    
    char *name;
    
} EagleDbSchema;

EagleDbSchema* EagleDbSchema_New(char *name);
void EagleDbSchema_Delete(EagleDbSchema *schema);
void EagleDbSchema_addTable(EagleDbSchema *schema, EagleDbTableData *td);

#endif
