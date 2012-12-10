#ifndef eagle_DBSuite_h
#define eagle_DBSuite_h

#include <CUnit/Basic.h>
#include "TestSuite.h"

extern int yyparse();
extern int yy_scan_string(const char *);

int DBSuite_init();
CUnitTests* DBSuite_tests();
int DBSuite_clean();

#endif
