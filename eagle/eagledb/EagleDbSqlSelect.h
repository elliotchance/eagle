#ifndef eagle_EagleDbSqlSelect_h
#define eagle_EagleDbSqlSelect_h

typedef struct {
    char *tableName;
} EagleDbSqlSelect;

EagleDbSqlSelect* EagleDbSqlSelect_New(void);

#endif
