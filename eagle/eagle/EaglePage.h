#ifndef eagle_EaglePage_h
#define eagle_EaglePage_h

#include "EagleBoolean.h"
#include "EagleData.h"
#include "Eagle.h"
#include "EagleDataType.h"

/**
 Represents a single page of data.
 */
typedef struct EaglePage {
    
    /**
     The data type for this page.
     */
    EAGLE_ATTR_NA EagleDataType type;
    
    /**
     An arbitrary counter for the row ID offset. The first record in this page will have \c recordOffset as the record
     ID, the next record will be recordOffset + 1, etc
     */
    EAGLE_ATTR_NA int recordOffset;
    
    /**
     The allocated records for this page.
     */
    EAGLE_ATTR_NA int totalSize;
    
    /**
     The amount of used records in this page.
     */
    EAGLE_ATTR_NA int count;
    
    /**
     The actual data for this page.
     */
    EAGLE_ATTR_SEMI_MANAGED void *data;
    
    /**
     Free \c data when freeing the page.
     */
    EAGLE_ATTR_NA EagleBoolean freeData;
    
    /**
     This value starts at 1 and is incremented with EaglePage_Copy() and decremented with EaglePage_Delete(). The page
     is only really freed when this counter hits zero.
     */
    EAGLE_ATTR_NA int usageCount;
    
} EaglePage;

/**
 Create a new EaglePage.
 You will be required to provide a data block with the number of records in that data block.
 
 @param [in] type The data for this page.
 @param [in] data The preallocated data block. This may be NULL, but the \p count must be 0.
 @param [in] totalSize The number of records in the data block.
 @param [in] count The number of used records (must be less than \p totalSize)
 @param [in] recordOffset An arbitrary number stored in the page. This number is used as an offset when calculating the
 record ID of a given record inside a page. If you are unsure as to what this value should be then you can use 0.
 @param [in] freeData If EagleTrue the \p data will be freed with the page.
 @return A new page.
 */
EaglePage* EaglePage_New(EagleDataType type, void *data, int totalSize, int count, int recordOffset, EagleBoolean freeData);

/**
 * Allocate a new page.
 * @param [in] type The data type.
 * @param [in] count The size of the page.
 * @return The new page.
 */
EaglePage* EaglePage_Alloc(EagleDataType type, int count);

/**
 * Allocate a new page of given size.
 * This function differs from EaglePage_New() because it will allocate the data block for you.
 * @note The page allocated will not be filled with any data, make sure you sanitize the page if you intend to read from
 * it.
 * @param [in] count The number of records of the page.
 * @return The new page.
 */
EaglePage* EaglePage_AllocInt(int count);

/**
 * Allocate a new page of given size.
 * This function differs from EaglePage_New() because it will allocate the data block for you.
 * @note The page allocated will not be filled with any data, make sure you sanitize the page if you intend to read from
 * it.
 * @param [in] count The number of records of the page.
 * @return The new page.
 */
EaglePage* EaglePage_AllocFloat(int count);

/**
 * Allocate a new page of given size.
 * This function differs from EaglePage_New() because it will allocate the data block for you.
 * @note The page allocated will not be filled with any data, make sure you sanitize the page if you intend to read from
 * it.
 * @param [in] count The number of records of the page.
 * @return The new page.
 */
EaglePage* EaglePage_AllocVarchar(int count);

/**
 * Copy a page.
 * @param [in] page The page to copy.
 * @return A duplicate page.
 */
EaglePage* EaglePage_Copy(EaglePage *page);

/**
 * Private function. Copy an INTEGER page.
 * @param [in] page The page.
 * @return A duplicate page.
 * @see EaglePage_Copy()
 */
EaglePage* EaglePage_RealCopyInt_(EaglePage *page);

/**
 * Private function. Copy an FLOAT page.
 * @param [in] page The page.
 * @return A duplicate page.
 * @see EaglePage_Copy()
 */
EaglePage* EaglePage_RealCopyFloat_(EaglePage *page);

/**
 * Private function. Copy a VARCHAR page.
 * @param [in] page The page.
 * @return A duplicate page.
 * @see EaglePage_Copy()
 */
EaglePage* EaglePage_RealCopyVarchar_(EaglePage *page);

/**
 * Free a page.
 * @param [in] page The page.
 */
void EaglePage_Delete(EaglePage *page);

/**
 * Render information about the page as a string.
 * @param [in] page The page.
 * @return A new string (you must free this yourself)
 */
char* EaglePage_toString(EaglePage *page);

EaglePage* EaglePage_RealCopy(EaglePage *page);

#endif
