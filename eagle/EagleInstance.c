#include <stdlib.h>
#include <stdio.h>
#include "EagleInstance.h"

EagleInstance* EagleInstance_New(int totalWorkers)
{
    EagleInstance *instance = (EagleInstance*) malloc(sizeof(EagleInstance));
    instance->workers = EagleWorkers_New(totalWorkers);
    return instance;
}

void EagleInstance_run(EagleInstance *eagle)
{
    EaglePlan *plan = eagle->plan;
    int stillRunning;
    do {
        stillRunning = 0;
        
        // load buffers
        for(int i = 0; i < plan->usedProviders; ++i) {
            EaglePlanBufferProvider *provider = plan->providers[i];
            printf("Pages Remaining (provider = %d): %d\n", i, EaglePageProvider_pagesRemaining(provider->provider));
            if(EaglePageProvider_pagesRemaining(provider->provider) > 0) {
                plan->buffers[provider->destinationBuffer] = EaglePageProvider_nextPage(provider->provider);
                ++stillRunning;
            }
        }
        
        // run operations
        for(int i = 0; i < plan->usedOperations; ++i) {
            EaglePlanOperation *epo = plan->operations[i];
            
            // execute page
            epo->function(plan->buffers[epo->source], plan->buffers[epo->destination], epo->obj);
        }
        
        // extract records
        for(int i = 0; i < plan->pageSize; ++i) {
            if(plan->buffers[0]->data[i]) {
                printf("%d\n", plan->buffers[plan->providers[0]->destinationBuffer]->data[i]);
            }
        }
    } while(stillRunning);
}

void EagleInstance_addPlan(EagleInstance *eagle, EaglePlan *plan)
{
    eagle->plan = plan;
}
