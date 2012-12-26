#include <stdio.h>
#include <stdlib.h>

#include "EagleInstance.h"
#include "EaglePageOperations.h"
#include "EaglePageProvider.h"
#include "EagleData.h"
#include "EaglePlan.h"
#include "EaglePlanOperation.h"
#include "EaglePlanBufferProvider.h"

extern int yyparse();
extern int yy_scan_string(const char *);

int main(int argc, const char * argv[])
{
    EagleInstance *eagle;
    int dataSize;
    int *data;
    int i;
    
    /* initialise workers */
    eagle = EagleInstance_New(8);
    
    /* prepare data */
    printf("Preparing data...\n");
    dataSize = 100000000;
    data = (int*) calloc((size_t) dataSize, sizeof(int));
    
    srand(0);
    for(i = 0; i < dataSize; ++i) {
        data[i] = (int) arc4random();
    }
    
    {
        clock_t start, end;
        
        /* plan: ? BETWEEN ? AND ? */
        int recordsPerPage = 10000;
        EaglePlan *plan = EaglePlan_New(recordsPerPage);
        EaglePageProvider *provider = EaglePageProvider_CreateFromIntArray(data, dataSize, recordsPerPage, NULL);
        EaglePlan_addBufferProvider(plan, EaglePlanBufferProvider_New(1, provider, EagleTrue));
        
        EaglePlan_addOperation(plan, EaglePlanOperation_New(EaglePageOperations_GreaterThanInt, 2, 1, -1, EagleData_Int(10000000), EagleTrue, "<2> > 5000000 -> <1>"));
        EaglePlan_addOperation(plan, EaglePlanOperation_New(EaglePageOperations_LessThanInt,    3, 1, -1, EagleData_Int(20000000), EagleTrue, "<3> < 10000000 -> <1>"));
        EaglePlan_addOperation(plan, EaglePlanOperation_New(EaglePageOperations_AndPage,        0, 2,  3, NULL, EagleFalse, "<2> && <3> -> DEST"));
        
        printf("%s", EaglePlan_toString(plan));
        
        EagleInstance_addPlan(eagle, plan);
        
        /* run */
        start = clock();
        EagleInstance_run(eagle);
        end = clock();
        printf("Total time: %.3f seconds\n", ((end - start) * 0.000001) / (float) eagle->workers->totalWorkers);
    }
    
    return 0;
}
