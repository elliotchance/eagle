#ifndef eagle_EagleDbInstance_h
#define eagle_EagleDbInstance_h

#include "EagleDbTableData.h"

typedef struct {
    
    EagleDbTableData *td;
    
} EagleDbInstance;

EagleDbInstance* EagleDbInstance_New(void);
void EagleDbInstance_Delete(EagleDbInstance *db);
void EagleDbInstance_execute(EagleDbInstance *db, char *sql);

#endif
