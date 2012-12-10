#ifndef eagle_EagleDbSqlValue_h
#define eagle_EagleDbSqlValue_h

typedef enum {
    EagleDbSqlValueTypeInteger = 1
} EagleDbSqlValueType;

typedef struct {
    EagleDbSqlValueType type;
    union {
        int intValue;
    } value;
} EagleDbSqlValue;

EagleDbSqlValue* EagleDbSqlValue_NewWithInteger(int value);

#endif
