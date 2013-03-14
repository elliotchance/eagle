#ifndef eagle_OperatorSuite_h
#define eagle_OperatorSuite_h

#include "TestSuite.h"
#include <CUnit/Basic.h>
#include "EaglePage.h"

#define OperatorSuite_ValidateBinary(_type1, _type2, _op) int valid = 1; \
for(int i = 0; i < pageSize; ++i) { \
if((((_type1*) page1->data)[i] _op ((_type1*) page2->data)[i]) != ((_type2*) out->data)[i]) { \
valid = 0; \
break; \
} \
}

#define OperatorSuite_ValidateUnary(_type, _op) int valid = 1; \
for(int i = 0; i < pageSize; ++i) { \
if((_op((_type*) page->data)[i]) != ((_type*) out->data)[i]) { \
valid = 0; \
break; \
} \
}

int OperatorSuite_init();
CUnitTests* OperatorSuite_tests();
int OperatorSuite_clean();

EaglePage* OperatorSuite_GeneratePageInt(int pageSize, int max);
EaglePage* OperatorSuite_GeneratePageFloat(int pageSize);

#endif
