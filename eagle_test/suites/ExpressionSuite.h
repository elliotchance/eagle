#ifndef eagle_ExpressionSuite_h
#define eagle_ExpressionSuite_h

#include <CUnit/Basic.h>
#include "ExpressionSuite.h"

extern int yyparse();
extern int yy_scan_string(const char *);

int ExpressionSuite_init();
CUnitTests* ExpressionSuite_tests();
int ExpressionSuite_clean();

#endif
