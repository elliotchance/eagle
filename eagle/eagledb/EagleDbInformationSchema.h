#ifndef eagle_EagleDbInformationSchema_h
#define eagle_EagleDbInformationSchema_h

#include "EaglePageProviderVirtual.h"
#include "EagleBoolean.h"
#include "EagleDbSchema.h"
#include "EagleDbInstance.h"

/**
 This object is used for callback functions on virtual tables.
 */
typedef struct {
    
    /**
     The DB instance.
     */
    EAGLE_ATTR_MANAGED EagleDbInstance *db;
    
    /**
     Record offset.
     */
    EAGLE_ATTR_NA int recordOffset;
    
    /**
     The column (field) name.
     */
    EAGLE_ATTR_MANAGED char *columnName;
    
} EagleDbInformationSchema;

/**
 Create a new EagleDbInformationSchema object.
 @param [in] db Instance.
 @param [in] columnName The column name.
 @return A new object.
 */
EagleDbInformationSchema* EagleDbInformationSchema_New(EagleDbInstance *db, char *columnName);

/**
 Initialise the virtual tables for the information_schema.
 @param [in] db Instance.
 @param [in] schema Schema to load virtual tables into.
 */
void EagleDbInformationSchema_Init(EagleDbInstance *db, EagleDbSchema *schema);

/**
 Remove/free the virtual tables.
 @param [in] db Instance.
 */
void EagleDbInformationSchema_Cleanup(EagleDbInstance *db);

/**
 Free a EagleDbInformationSchema.
 @param [in] infoSchema Information schema.
 */
void EagleDbInformationSchema_Delete(EagleDbInformationSchema *infoSchema);

/**
 Free.
 @param [in] infoSchema Information schema.
 */
void EagleDbInformationSchema_tables_free(EagleDbInformationSchema *infoSchema);

/**
 The number of pages remaining.
 @param [in] infoSchema Information schema.
 @return The number of pages remaining.
 */
int EagleDbInformationSchema_tables_pagesRemaining(EagleDbInformationSchema *infoSchema);

/**
 Return the next page.
 @param [in] infoSchema Information schema.
 @return The next page.
 */
EaglePage* EagleDbInformationSchema_tables_nextPage(EagleDbInformationSchema *infoSchema);

/**
 Reset the cursor.
 @param [in] infoSchema Information schema.
 */
void EagleDbInformationSchema_tables_reset(EagleDbInformationSchema *infoSchema);

#endif
