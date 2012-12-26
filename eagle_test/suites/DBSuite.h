#ifndef eagle_DBSuite_h
#define eagle_DBSuite_h

#include <CUnit/Basic.h>
#include "TestSuite.h"
#include "EagleDbSqlExpression.h"

extern int yyparse();
extern int yy_scan_string(const char *);

int DBSuite_init();
CUnitTests* DBSuite_tests();
int DBSuite_clean();
EagleDbSqlExpression* _getExpression(const char *sql);

#endif
