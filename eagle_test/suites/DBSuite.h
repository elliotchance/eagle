#ifndef eagle_DBSuite_h
#define eagle_DBSuite_h

#include "TestSuite.h"
#include <CUnit/Basic.h>
#include "EagleDbSqlExpression.h"

int DBSuite_init();
CUnitTests* DBSuite_tests();
int DBSuite_clean();
EagleDbSqlExpression* _getExpression(const char *sql);

#endif
