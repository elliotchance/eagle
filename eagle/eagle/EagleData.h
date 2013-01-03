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

int* EagleData_Int(int value);

EagleDataType EagleDataType_nameToType(char *name);
char* EagleDataType_typeToName(EagleDataType type);

#endif
