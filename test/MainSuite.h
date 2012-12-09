#ifndef eagle_MainSuite_h
#define eagle_MainSuite_h

#include <CUnit/Basic.h>
#include "TestSuite.h"
#include "EaglePage.h"

int MainSuite_init();
CUnitTests* MainSuite_tests();
int MainSuite_clean();

EaglePage* MainSuite_GeneratePage(int pageSize);

CUNIT_TEST(MainSuite, testPageGreaterThanInt);
CUNIT_TEST(MainSuite, testPageLessThanInt);
CUNIT_TEST(MainSuite, testPageAnd);

#endif
