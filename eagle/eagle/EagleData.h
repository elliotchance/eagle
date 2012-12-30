#ifndef eagle_EagleData_h
#define eagle_EagleData_h

/**
 Data types for pages.
 */
typedef enum {
    
    /**
     Unknown/invalid data type.
     */
    EagleDataTypeUnknown = 0,
    
    /**
     32bit integer.
     */
    EagleDataTypeInteger = 1,
    
    /**
     String.
     */
    EagleDataTypeText = 2
    
} EagleDataType;

int* EagleData_Int(int value);

EagleDataType EagleDataType_nameToType(char *name);
char* EagleDataType_typeToName(EagleDataType type);

#endif
