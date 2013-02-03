#ifndef eagle_EaglePageOperations_h
#define eagle_EaglePageOperations_h

/**
 Function pointer.
 */
#define EaglePageOperationFunction(ptr) void (*ptr)(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)

#include "EaglePage.h"

/**
 Calculate values in a page that are less than an integer.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2. All records in this page
 will be given a value or 0 or 1 based on the result of the comparison.
 @param [in] source1 The page to scan, can be any size.
 @param [in] source2 Ignored.
 @param [in] obj Must be int* which is dereferenced to the integer value for comparison.
 
 @see EagleData_Int()
 */
void EaglePageOperations_LessThanInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Calculate values in a page that are greater than an integer.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2. All records in this page
 will be given a value or 0 or 1 based on the result of the comparison.
 @param [in] source1 The page to scan, can be any size.
 @param [in] source2 Ignored.
 @param [in] obj Must be int* which is dereferenced to the integer value for comparison.
 
 @see EagleData_Int()
 */
void EaglePageOperations_GreaterThanInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Cast integer values to boolean (0 and 1).
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2. All records in this page
 will be given a value or 0 or 1.
 @param [in] source1 The page to scan, can be any size.
 @param [in] source2 Ignored.
 @param [in] obj Ignored.
 */
void EaglePageOperations_CastIntToBool(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Calculate logical AND between two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2. All records in this page
 will be given a value or 0 or 1 based on the result of the comparison.
 @param [in] source1 Left operand.
 @param [in] source2 Right operand.
 @param [in] obj Ignored.
 */
void EaglePageOperations_AndPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Addition of two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_AdditionPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Equals comparison between two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_EqualsPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Send records to another provider based on a condition.
 
 @param [in] destination Ignored.
 @param [in] source1 A integer (boolean) to decide if the value should be sent to the provider.
 @param [in] source2 The value to be sent to the provider.
 @param [in] obj An instance of EaglePageProvider.
 */
void EaglePageOperations_SendPageToProvider(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Modulus of two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_ModulusPage(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

#endif
