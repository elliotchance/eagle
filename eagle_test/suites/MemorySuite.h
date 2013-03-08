#ifndef eagle_MemorySuite_h
#define eagle_MemorySuite_h

#include "TestSuite.h"
#include <CUnit/Basic.h>

int MemorySuite_init();
CUnitTests* MemorySuite1_tests();
CUnitTests* MemorySuite2_tests();
int MemorySuite_clean();

#endif
