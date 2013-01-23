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

/**
 Create a new table column.
 
 @param [in] name The name will be duplicated.
 @param [in] type The data type for this column.
 */
EagleDbColumn* EagleDbColumn_New(char *name, EagleDataType type);

/**
 Free a column.
 */
void EagleDbColumn_Delete(EagleDbColumn *column);

#endif
