#ifndef eagle_EagleDbColumn_h
#define eagle_EagleDbColumn_h

/**
 Each column in a table must have a data type.
 */
typedef enum {
    
    /**
     32bit integer.
     */
    EagleDbColumnTypeInteger = 1
    
} EagleDbColumnType;

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
    EagleDbColumnType type;
    
} EagleDbColumn;

EagleDbColumn* EagleDbColumn_New(char *name, EagleDbColumnType type);
void EagleDbColumn_Delete(EagleDbColumn *column);

#endif
