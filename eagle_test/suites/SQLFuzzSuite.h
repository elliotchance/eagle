#ifndef eagle_SQLFuzzSuite_h
#define eagle_SQLFuzzSuite_h

#include "TestSuite.h"
#include <CUnit/Basic.h>

int SQLFuzzSuite_init();
CUnitTests* SQLFuzzSuite_tests();
int SQLFuzzSuite_clean();

#endif
