#ifndef eagle_EagleDbTuple_h
#define eagle_EagleDbTuple_h

#include "EagleDbColumn.h"
#include "EagleDbTable.h"

typedef struct {
    
    void **data;
    
    EagleDbTable *table;
    
} EagleDbTuple;

EagleDbTuple* EagleDbTuple_New(EagleDbTable *table);
void EagleDbTuple_Delete(EagleDbTuple *tuple);

void EagleDbTuple_setInt(EagleDbTuple *tuple, int position, int value);
void EagleDbTuple_setText(EagleDbTuple *tuple, int position, char *value);
char* EagleDbTuple_toString(EagleDbTuple *tuple);

#endif
