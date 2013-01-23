#ifndef eagle_EagleDbTuple_h
#define eagle_EagleDbTuple_h

#include "EagleDbColumn.h"
#include "EagleDbTable.h"
#include "Eagle.h"

typedef struct {
    
    /**
     The data contained in the tuple. Do not access this directy, use appopriate methods.
     */
    EAGLE_ATTR_MANAGED void **data;
    
    /**
     A reference to the table structure that the tuple is based on.
     */
    EAGLE_ATTR_PROVIDED EagleDbTable *table;
    
} EagleDbTuple;

EagleDbTuple* EagleDbTuple_New(EagleDbTable *table);

void EagleDbTuple_Delete(EagleDbTuple *tuple);

void EagleDbTuple_setInt(EagleDbTuple *tuple, int position, int value);

void EagleDbTuple_setText(EagleDbTuple *tuple, int position, char *value);

char* EagleDbTuple_toString(EagleDbTuple *tuple);

#endif
