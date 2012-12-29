#ifndef eagle_EaglePage_h
#define eagle_EaglePage_h

#include "EagleBoolean.h"
#include "EagleData.h"

/**
 Represents a single page of data.
 */
struct EaglePage_ {
    
    EagleDataType type;
    
    /**
     An arbitrary counter for the row ID offset. The first record in this page will have \c recordOffset as the record
     ID, the next record will be recordOffset + 1, etc
     */
    int recordOffset;
    
    /**
     The total amount of records in this page.
     */
    int count;
    
    /**
     The actual data for this page.
     */
    void *data;
    
    /**
     Free \c data when freeing the page.
     */
    EagleBoolean freeData;
    
};
typedef struct EaglePage_ EaglePage;

EaglePage* EaglePage_New(EagleDataType type, void *data, int count, int recordOffset, EagleBoolean freeData);
EaglePage* EaglePage_Alloc(EagleDataType type, int count);
EaglePage* EaglePage_AllocInt(int count);
EaglePage* EaglePage_AllocText(int count);
EaglePage* EaglePage_Copy(EaglePage *page);
EaglePage* EaglePage_CopyInt_(EaglePage *page);
EaglePage* EaglePage_CopyText_(EaglePage *page);
void EaglePage_Delete(EaglePage *page);

#endif
