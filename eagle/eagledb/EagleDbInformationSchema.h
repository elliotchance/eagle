#ifndef eagle_EagleDbInformationSchema_h
#define eagle_EagleDbInformationSchema_h

#include "EaglePageProviderVirtual.h"
#include "EagleBoolean.h"
#include "EagleDbSchema.h"
#include "EagleDbInstance.h"

typedef struct {
    
    EAGLE_ATTR_MANAGED EagleDbInstance *db;
    
    EAGLE_ATTR_NA int recordOffset;
    
    EAGLE_ATTR_MANAGED const char *columnName;
    
} EagleDbInformationSchema;

EagleDbInformationSchema* EagleDbInformationSchema_New(EagleDbInstance *db, const char *columnName);

void EagleDbInformationSchema_Init(EagleDbInstance *db, EagleDbSchema *schema);

void EagleDbInformationSchema_Delete(EagleDbInformationSchema *infoSchema);

/* --- */

void EagleDbInformationSchema_tables_free(EagleDbInformationSchema *infoSchema);

int EagleDbInformationSchema_tables_pagesRemaining(EagleDbInformationSchema *infoSchema);

EaglePage* EagleDbInformationSchema_tables_nextPage(EagleDbInformationSchema *infoSchema);

void EagleDbInformationSchema_tables_reset(EagleDbInformationSchema *infoSchema);

#endif
