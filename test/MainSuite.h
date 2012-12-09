#ifndef eagle_MainSuite_h
#define eagle_MainSuite_h

#include <CUnit/Basic.h>
#include "TestSuite.h"

int MainSuite_init();
CUnitTests* MainSuite_tests();
int MainSuite_clean();

CUNIT_TEST(MainSuite, testPageGreaterThanInt);

#endif
