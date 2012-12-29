#ifndef eagle_EagleDbColumn_h
#define eagle_EagleDbColumn_h

#include "EagleData.h"

/**
 The definition for a table column.
 */
typedef struct {
    
    /**
     The name of the column.
     */
    char *name;
    
    /**
     Column data type.
     */
    EagleDataType type;
    
} EagleDbColumn;

EagleDbColumn* EagleDbColumn_New(char *name, EagleDataType type);
void EagleDbColumn_Delete(EagleDbColumn *column);

#endif
