#include <stdlib.h>
#include <string.h>
#include "EaglePlan.h"

EaglePlan* EaglePlan_New(int pageSize)
{
    EaglePlan *plan = (EaglePlan*) malloc(sizeof(EaglePlan));
    
    plan->pageSize = pageSize;
    
    plan->allocatedOperations = 10;
    plan->usedOperations = 0;
    plan->operations = (EaglePlanOperation**) calloc(plan->allocatedOperations, sizeof(EaglePlanOperation*));
    
    plan->allocatedProviders = 10;
    plan->usedProviders = 0;
    plan->providers = (EaglePlanBufferProvider**) calloc(plan->allocatedProviders, sizeof(EaglePlanBufferProvider*));
    
    return plan;
}

void EaglePlan_addOperation(EaglePlan *plan, EaglePlanOperation *epo)
{
    plan->operations[plan->usedOperations++] = epo;
}

void EaglePlan_addBufferProvider(EaglePlan *plan, EaglePlanBufferProvider *bp)
{
    plan->providers[plan->usedProviders++] = bp;
}

const char* EaglePlan_toString(EaglePlan *plan)
{
    char *str = (char*) malloc(1024);
    str[0] = 0;
    strcat(str, "EaglePlan:\n");
    for(int i = 0; i < plan->usedOperations; ++i) {
        strcat(str, "  ");
        strcat(str, EaglePlanOperation_toString(plan->operations[i]));
        strcat(str, "\n");
    }
    return str;
}
