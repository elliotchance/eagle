#ifndef eagle_EagleData_h
#define eagle_EagleData_h

#include "EagleDataType.h"

/**
 Allocate an integer and return the pointer.
 
 @param [in] value Value to set into newly allocated space.
 @return New allocation containing the integer.
 */
EagleDataTypeIntegerType* EagleData_Int(EagleDataTypeIntegerType value);

/**
 Allocate a float and return the pointer.
 
 @param [in] value Value to set into newly allocated space.
 @return New allocation containing the float.
 */
EagleDataTypeFloatType* EagleData_Float(EagleDataTypeFloatType value);

#endif
