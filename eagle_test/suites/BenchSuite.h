#ifndef eagle_BenchSuite_h
#define eagle_BenchSuite_h

#include "TestSuite.h"
#include <CUnit/Basic.h>

int BenchSuite_init();
CUnitTests* BenchSuite_tests();
int BenchSuite_clean();

#endif
