#ifndef eagle_EagleDbSqlSelect_h
#define eagle_EagleDbSqlSelect_h

#include "EagleDbSqlValue.h"

typedef struct {
    char *tableName;
    EagleDbSqlValue *whereExpression;
} EagleDbSqlSelect;

EagleDbSqlSelect* EagleDbSqlSelect_New(void);

#endif
