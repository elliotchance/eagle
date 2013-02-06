#ifndef eagle_MainSuite_h
#define eagle_MainSuite_h

#include "TestSuite.h"
#include <CUnit/Basic.h>
#include "EaglePage.h"

int MainSuite_init();
CUnitTests* MainSuite_tests();
int MainSuite_clean();

#endif
