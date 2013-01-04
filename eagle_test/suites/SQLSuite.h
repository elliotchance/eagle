#ifndef eagle_SQLSuite_h
#define eagle_SQLSuite_h

#include "TestSuite.h"
#include <CUnit/Basic.h>

extern int yyparse();
extern int yy_scan_string(const char *);

int SQLSuite_init();
CUnitTests* SQLSuite_tests();
int SQLSuite_clean();

#endif
