#ifndef eagle_EagleData_h
#define eagle_EagleData_h

/**
 Data types for pages.
 */
typedef enum {
    
    EagleDataTypeUnknown = 0, /**< Unknown/invalid data type. */
    EagleDataTypeInteger = 1, /**< \c INT or \c INTEGER: 32bit integer. */
    EagleDataTypeText    = 2  /**< \c TEXT: A NULL terminated string. */
    
} EagleDataType;

/**
 Allocate an integer and return the pointer.
 
 @param [in] value Value to set into newly allocated space.
 */
int* EagleData_Int(int value);

/**
 Convert a type name to its enum value. This function is not case-sensitive.
 
 @param [in] name The name of the type.
 @return EagleDataTypeUnknown if the type is unknown. Otherwise the correct enum value for the type.
 @see EagleDataType
 */
EagleDataType EagleDataType_nameToType(char *name);

/**
 Convert a type to a string.
 
 @param [in] type The data type.
 @return A new string (you must free it)
 @see EagleDataType
 */
char* EagleDataType_typeToName(EagleDataType type);

#endif
