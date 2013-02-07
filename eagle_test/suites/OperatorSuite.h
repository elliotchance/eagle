#ifndef eagle_OperatorSuite_h
#define eagle_OperatorSuite_h

#include "TestSuite.h"
#include <CUnit/Basic.h>
#include "EaglePage.h"

int OperatorSuite_init();
CUnitTests* OperatorSuite_tests();
int OperatorSuite_clean();

EaglePage* OperatorSuite_GeneratePage(int pageSize);

#endif
