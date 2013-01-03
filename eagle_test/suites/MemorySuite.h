#ifndef eagle_MemorySuite_h
#define eagle_MemorySuite_h

#include <CUnit/Basic.h>
#include "TestSuite.h"

int MemorySuite_init();
CUnitTests* MemorySuite_tests();
int MemorySuite_clean();

#endif
