#include <stdio.h>
#include <stdlib.h>
#include "MemorySuite.h"
#include "EagleMemory.h"
#include "EagleData.h"
#include "EagleDbColumn.h"
#include "EagleDbConsole.h"

CUNIT_TEST(MemorySuite, EagleData_Int)
{
    EagleMemory_MockInit();
    EagleMemory_Mock("EagleData_Int.1");
    
    CUNIT_ASSERT_NULL(EagleData_Int(123));
    
    EagleMemory_MockFinish();
}

CUNIT_TEST(MemorySuite, EagleDbColumn_New)
{
    EagleMemory_MockInit();
    EagleMemory_Mock("EagleDbColumn_New.1");
    
    CUNIT_ASSERT_NULL(EagleDbColumn_New("abc", EagleDataTypeInteger));
    
    EagleMemory_MockFinish();
}

CUNIT_TEST(MemorySuite, EagleDbConsole_New)
{
    EagleMemory_MockInit();
    EagleMemory_Mock("EagleDbConsole_New.1");
    
    CUNIT_ASSERT_NULL(EagleDbConsole_New());
    
    EagleMemory_MockFinish();
}

CUNIT_TEST(MemorySuite, EagleDbConsole_GetLine)
{
    EagleMemory_MockInit();
    EagleMemory_Mock("EagleDbConsole_GetLine.1");
    
    CUNIT_ASSERT_NULL(EagleDbConsole_GetLine());
    
    EagleMemory_MockFinish();
}

/**
 * The suite init function.
 */
int MemorySuite_init()
{
    return 0;
}

/**
 * The suite cleanup function.
 */
int MemorySuite_clean()
{
    return 0;
}

CUnitTests* MemorySuite_tests()
{
    CUnitTests *tests = CUnitTests_New(100);
    
    // method tests
    CUnitTests_addTest(tests, CUNIT_NEW(MemorySuite, EagleData_Int));
    CUnitTests_addTest(tests, CUNIT_NEW(MemorySuite, EagleDbColumn_New));
    CUnitTests_addTest(tests, CUNIT_NEW(MemorySuite, EagleDbConsole_New));
    CUnitTests_addTest(tests, CUNIT_NEW(MemorySuite, EagleDbConsole_GetLine));
    
    return tests;
}
