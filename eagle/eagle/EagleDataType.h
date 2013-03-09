#ifndef eagle_EagleDataType_h
#define eagle_EagleDataType_h

/**
 Data types for pages.
 */
typedef enum {
    
    EagleDataTypeUnknown = 0,    /**< Unknown/invalid data type. */
    
    EagleDataTypeInteger = 1,    /**< \c INT or \c INTEGER: 32bit integer. */
    
    EagleDataTypeVarchar = 2,    /**< \c VARCHAR: A NULL terminated string. */
    
    EagleDataTypeFloat = 3       /**< \c FLOAT: 64bit floating point. */
    
} EagleDataType;

/**
 The global data type for handling EagleDataTypeInteger.
 */
typedef int EagleDataTypeIntegerType;

/**
 The global data type for handling EagleDataTypeVarchar.
 */
typedef char* EagleDataTypeVarcharType;

/**
 The global data type for handling EagleDataTypeFloat.
 */
typedef double EagleDataTypeFloatType;

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
