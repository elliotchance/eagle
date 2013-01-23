#ifndef eagle_EaglePageOperations_h
#define eagle_EaglePageOperations_h

#define EaglePageOperationFunction(ptr) void (*ptr)(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)

#include "EaglePage.h"

/**
 Calculate values in a page that are less than an integer.
 
 This function does not allocate memory.
 
 @param [in] destination Must be preallocated to the same size as \p page. All records in this page will be given a
 value or 0 or 1 based on the result of the comparison.
 @param [in] source1 The page to scan, can be any size.
 @param [in] source2 Ignored.
 @param [in] obj Must be int* which is dereferenced to the integer value for comparison.
 
 @see EagleData_Int()
 */
void EaglePageOperations_LessThanInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Calculate values in a page that are greater than an integer.
 
 This function does not allocate memory.
 
 @param [in] destination Must be preallocated to the same size as \p page. All records in this page will be given a
 value or 0 or 1 based on the result of the comparison.
 @param [in] source1 The page to scan, can be any size.
 @param [in] source2 Ignored.
 @param [in] obj Must be int* which is dereferenced to the integer value for comparison.
 
 @see EagleData_Int()
 */
void EaglePageOperations_GreaterThanInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

void EaglePageOperations_CastIntToBool(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Calculate logical AND between two pages.
 
 @param [in] destination Must be preallocated to the same size as \p page. All records in this page will be given a
 value or 0 or 1 based on the result of the comparison.
 @param [in] source1 Left operand.
 @param [in] source2 Right operand.
 @param [in] obj Ignored.
 */
void EaglePageOperations_AndPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

void EaglePageOperations_AdditionPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

void EaglePageOperations_EqualsPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

void EaglePageOperations_CastIntPageToBoolean(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

void EaglePageOperations_SendPageToProvider(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

void EaglePageOperations_ModulusPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

#endif
