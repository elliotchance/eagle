#ifndef eagle_EagleDbSchema_h
#define eagle_EagleDbSchema_h

#include "EagleDbTable.h"

typedef struct {
    
    int allocatedTables;
    
    int usedTables;
    
    EagleDbTable **tables;
    
    char *name;
    
} EagleDbSchema;

EagleDbSchema* EagleDbSchema_New(char *name);
void EagleDbSchema_Delete(EagleDbSchema *schema);

#endif
