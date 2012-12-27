#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "EagleDbConsole.h"
#include "EagleDbInstance.h"

EagleDbConsole* EagleDbConsole_New(void)
{
    EagleDbConsole *console = (EagleDbConsole*) malloc(sizeof(EagleDbConsole));
    
    console->startTime = mach_absolute_time();
    
    return console;
}

/**
 @url http://stackoverflow.com/a/314422/1470961
 */
char* EagleDbConsole_GetLine(void)
{
    char *line = malloc(100), *linep = line, *linen = NULL;
    size_t lenmax = 100, len = lenmax;
    int c;
    
    if(line == NULL) {
        return NULL;
    }
    
    while(1) {
        c = fgetc(stdin);
        if(c == EOF) {
            break;
        }
        
        if(--len == 0) {
            len = lenmax;
            linen = realloc(linep, lenmax *= 2);
            
            if(linen == NULL) {
                free(linep);
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
}

void EagleDbConsole_run(EagleDbConsole *console)
{
    char *cmd = NULL;
    EagleDbInstance *db = EagleDbInstance_New();
    
    while(1) {
        /* get line */
        printf("eagle> ");
        cmd = EagleDbConsole_GetLine();
        
        /* check for quit */
        if(NULL != cmd && strcmp(cmd, "\\q") == 0) {
            printf("Bye.\n");
            free(cmd);
            break;
        }
        
        /* parse */
        EagleDbInstance_execute(db, cmd);
        free(cmd);
    }
}

void EagleDbConsole_Delete(EagleDbConsole *console)
{
    free(console);
}
