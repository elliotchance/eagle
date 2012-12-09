#include <stdio.h>
#include <stdlib.h>

#include "EagleInstance.h"
#include "EaglePageOperations.h"
#include "EaglePageReceiver.h"
#include "EaglePageProvider.h"
#include "EagleData.h"
#include "EaglePlan.h"
#include "EaglePlanOperation.h"
#include "EaglePlanBufferProvider.h"

int main(int argc, const char * argv[])
{
    // initialise workers
    EagleInstance *eagle = EagleInstance_New(3);
    
    // prepare data
    printf("Preparing data...\n");
    int dataSize = 10000;
    int *data = (int*) calloc(sizeof(int), dataSize);
    
    srand(0);
    for(int i = 0; i < dataSize; ++i) {
        data[i] = rand();
    }
    
    // plan: ? BETWEEN ? AND ?
    EaglePlan *plan = EaglePlan_New(1000);
    EaglePageProvider *provider = EaglePageProvider_CreateFromIntStream(data, dataSize);
    EaglePlan_addBufferProvider(plan, EaglePlanBufferProvider_New(1, provider));
    
    EaglePlan_addOperation(plan, EaglePlanOperation_New(2, EaglePageOperations_GreaterThanInt, 1, EagleData_Int(10000000), "<2> > 5000000 -> <1>"));
    EaglePlan_addOperation(plan, EaglePlanOperation_New(3, EaglePageOperations_LessThanInt,    1, EagleData_Int(20000000), "<3> < 10000000 -> <1>"));
    EaglePlan_addOperation(plan, EaglePlanOperation_NewPage(0, EaglePageOperations_AndPage,    2, 3,                       "<2> && <3> -> DEST"));
    
    printf("%s", EaglePlan_toString(plan));
    
    EagleInstance_addPlan(eagle, plan);
    
    // run
    EagleInstance_run(eagle);
    
    return 0;
}
