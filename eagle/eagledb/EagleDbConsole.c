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
    EagleBoolean eof = EagleFalse;
#endif
    
    if(line == NULL) {
        return NULL;
    }
    
#ifndef CUNIT
    while(1) {
        c = fgetc(stdin);
        if(c == EOF) {
            eof = EagleTrue;
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
    
    /* if there is nothing left on the stream return NULL */
    if(EagleTrue == eof && strlen(linep) == 0) {
        return NULL;
    }
    
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
    EagleDbInstance *db = EagleDbInstance_New(1000, 1 /* FIXME */);
    EagleLoggerEvent *error = NULL;
    EagleBoolean readStdin = EagleFalse;
    
    /* read stdin (this is for a file being redriected in) */
    while(NULL != (cmd = EagleDbConsole_GetLine())) {
        /* parse */
        error = NULL;
        EagleDbInstance_execute(db, cmd, &error);
        if(NULL != error) {
            printf("Error: %s\n\n", error->message);
        }
        
        EagleMemory_Free(cmd);
        readStdin = EagleTrue;
    }
    
    /* now read commands */
    if(EagleFalse == readStdin) {
        while(1) {
            /* get line */
            printf("eagle> ");
            cmd = EagleDbConsole_GetLine();
            
            /* broken input */
            if(NULL == cmd) {
                printf("Error: Broken stdin.\n");
                break;
            }
            
            /* check for quit */
            if(strcmp(cmd, "\\q") == 0) {
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
