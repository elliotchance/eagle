#ifndef eagle_EagleDbSqlValueType_h
#define eagle_EagleDbSqlValueType_h

/**
 A value can contain several types.
 
 The first member variable of EagleDbSqlValue will indicate the real type of the Value.
 */
typedef enum {
    
    /**
     Use \p intValue
     */
    EagleDbSqlValueTypeInteger = 1,
    
    /**
     Asterisk expression for selecting multiple columns.
     */
    EagleDbSqlValueTypeAsterisk = 2,
    
    /**
     The name of a column.
     */
    EagleDbSqlValueTypeIdentifier = 3,
    
    /**
     A string literal.
     */
    EagleDbSqlValueTypeString = 4,
    
    /**
     A floating point number.
     */
    EagleDbSqlValueTypeFloat = 5
    
} EagleDbSqlValueType;

#endif
