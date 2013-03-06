#ifndef eagle_EagleDbTuple_h
#define eagle_EagleDbTuple_h

#include "EagleDbColumn.h"
#include "EagleDbTable.h"
#include "Eagle.h"

/**
 A tuple represents a row in a table.
 */
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

/**
 * Create a new tuple (record).
 * @param [in] table Table instance.
 * @return The newly created tuple for \p table definition.
 */
EagleDbTuple* EagleDbTuple_New(EagleDbTable *table);

/**
 * Delete a tuple.
 * @param [in] tuple The tuple instance.
 */
void EagleDbTuple_Delete(EagleDbTuple *tuple);

/**
 * Set an INTEGER value to a column in a tuple.
 * @param [in] tuple The tuple instance.
 * @param [in] position The column position, the first column will be index 0.
 * @param [in] value The value.
 */
void EagleDbTuple_setInt(EagleDbTuple *tuple, int position, int value);

/**
 * Set a VARCHAR value to a column in a tuple.
 * @param [in] tuple The tuple instance.
 * @param [in] position The column position, the first column will be index 0.
 * @param [in] value The value.
 */
void EagleDbTuple_setVarchar(EagleDbTuple *tuple, int position, const char *value);

/**
 * Render a tuple as a string.
 * @param [in] tuple The tuple instance.
 * @return A new string instance.
 */
char* EagleDbTuple_toString(EagleDbTuple *tuple);

#endif
