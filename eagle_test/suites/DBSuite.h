#ifndef eagle_DBSuite_h
#define eagle_DBSuite_h

#include "TestSuite.h"
#include <CUnit/Basic.h>
#include "EagleDbSqlExpression.h"
#include "EagleDbInstance.h"

int DBSuite_init();
CUnitTests* DBSuite1_tests();
CUnitTests* DBSuite2_tests();
int DBSuite_clean();
EagleDbSqlExpression* _getExpression(const char *sql);
EagleDbInstance* EagleInstanceTest(int pageSize);
void EagleInstanceTest_Cleanup(EagleDbInstance* db);

#endif
