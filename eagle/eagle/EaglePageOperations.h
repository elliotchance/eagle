#ifndef eagle_EaglePageOperations_h
#define eagle_EaglePageOperations_h

#include "EaglePage.h"

/**
 Function pointer.
 */
#define EaglePageOperationFunction(ptr) void (*ptr)(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj)

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
void EaglePageOperations_AndPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Calculate logical OR between two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2. All records in this page
 will be given a value or 0 or 1 based on the result of the comparison.
 @param [in] source1 Left operand.
 @param [in] source2 Right operand.
 @param [in] obj Ignored.
 */
void EaglePageOperations_OrPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Negate page.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2. All records in this page
 will be given a value or 0 or 1 based on the result of the comparison.
 @param [in] source1 Operand.
 @param [in] source2 Ignored.
 @param [in] obj Ignored.
 */
void EaglePageOperations_NegatePageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Negate page.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2. All records in this page
 will be given a value or 0 or 1 based on the result of the comparison.
 @param [in] source1 Operand.
 @param [in] source2 Ignored.
 @param [in] obj Ignored.
 */
void EaglePageOperations_NegatePageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Calculate logical NOT of a page.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2. All records in this page
 will be given a value or 0 or 1 based on the result of the comparison.
 @param [in] source1 Operand.
 @param [in] source2 Ignored.
 @param [in] obj Ignored.
 */
void EaglePageOperations_NotPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Addition of two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_AdditionPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Addition of two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_AdditionPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Multiplication of two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_MultiplyPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Multiplication of two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_MultiplyPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Equals comparison between two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_EqualsPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Equals comparison between two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_EqualsPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Not equals comparison between two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_NotEqualsPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Not equals comparison between two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_NotEqualsPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

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
void EaglePageOperations_ModulusPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Greater than comparison between two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_GreaterThanPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Greater than comparison between two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_GreaterThanPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Less than comparison between two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_LessThanPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Less than comparison between two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_LessThanPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Greater than or equal to comparison between two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_GreaterThanEqualPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Greater than or equal to comparison between two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_GreaterThanEqualPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Less than or equal to comparison between two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_LessThanEqualPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Less than or equal to comparison between two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_LessThanEqualPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Subtraction (minus) of two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_SubtractPageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Subtraction (minus) of two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_SubtractPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Division of two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_DividePageInt(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Division of two pages.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2.
 @param [in] source1 Operand 1.
 @param [in] source2 Operand 2.
 @param [in] obj Ignored.
 */
void EaglePageOperations_DividePageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

/**
 Calculate square root of a page.
 
 @param [in] destination Must be preallocated to the same size as \p source1 and \p source2. All records in this page
 will be given a value or 0 or 1 based on the result of the comparison.
 @param [in] source1 Operand.
 @param [in] source2 Ignored.
 @param [in] obj Ignored.
 */
void EaglePageOperations_SqrtPageFloat(EaglePage *destination, EaglePage *source1, EaglePage *source2, void *obj);

#endif
