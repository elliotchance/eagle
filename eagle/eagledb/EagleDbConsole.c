#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "EagleDbConsole.h"
#include "EagleDbInstance.h"
#include "EagleMemory.h"
#include "EagleLogger.h"
#include "EagleLoggerEvent.h"

EagleDbConsole* EagleDbConsole_New(void)
{
    EagleDbConsole *console = (EagleDbConsole*) EagleMemory_Allocate("EagleDbConsole_New.1", sizeof(EagleDbConsole));
    if(NULL == console) {
        return NULL;
    }
    
    console->startTime = mach_absolute_time();
    
    return console;
}

char* EagleDbConsole_GetLine(void)
{
    char *line = EagleMemory_Allocate("EagleDbConsole_GetLine.1", 100);
#ifndef CUNIT
    char *linep = line, *linen = NULL;
    size_t lenmax = 100, len = lenmax;
    int c;
#endif
    
    if(line == NULL) {
        return NULL;
    }
    
#ifndef CUNIT
    while(1) {
        c = fgetc(stdin);
        if(c == EOF) {
            break;
        }
        
        if(--len == 0) {
            len = lenmax;
            linen = realloc(linep, lenmax *= 2);
            
            if(linen == NULL) {
                EagleMemory_Free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }
        
        if((*line++ = (char) c) == '\n') {
            break;
        }
    }
    *line = '\0';
    
    /* chomp */
    linep[strlen(linep) - 1] = '\0';
    
    return linep;
#else
    EagleMemory_Free(line);
    return NULL;
#endif
}

void EagleDbConsole_run(EagleDbConsole *console)
{
#ifndef CUNIT
    char *cmd = NULL;
    EagleDbInstance *db = EagleDbInstance_New(1000);
    EagleLoggerEvent *error = NULL;
    
    while(1) {
        /* get line */
        printf("eagle> ");
        cmd = EagleDbConsole_GetLine();
        
        /* check for quit */
        if(NULL != cmd && strcmp(cmd, "\\q") == 0) {
            printf("Bye.\n");
            EagleMemory_Free(cmd);
            break;
        }
        
        /* parse */
        error = NULL;
        EagleDbInstance_execute(db, cmd, &error);
        if(NULL != error) {
            printf("Error: %s\n\n", error->message);
        }
        
        EagleMemory_Free(cmd);
    }
#endif
}

void EagleDbConsole_Delete(EagleDbConsole *console)
{
    if(NULL == console) {
        return;
    }
    
#ifndef CUNIT
    /* make sure the log is closed cleanly */
    EagleLogger_Delete(EagleLogger_Get());
#endif
    
    EagleMemory_Free(console);
}
