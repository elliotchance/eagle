#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "EagleDbInstance.h"
#include "EagleDbTable.h"
#include "EagleDbTuple.h"
#include "EagleDbSqlSelect.h"
#include "EaglePageProvider.h"
#include "EagleInstance.h"

extern void *yyparse_ast;
extern int yyparse();
extern int yy_scan_string(const char *);

char* yyerrors_last();
void yylex_init();
void yylex_free();

EagleDbInstance* EagleDbInstance_New(void)
{
    int i;
    EagleDbInstance *db = (EagleDbInstance*) malloc(sizeof(EagleDbInstance));
    
    /* create a virtual table */
    EagleDbTable *table = EagleDbTable_New("t");
    EagleDbTable_addColumn(table, EagleDbColumn_New("col1", EagleDbColumnTypeInteger));
    EagleDbTable_addColumn(table, EagleDbColumn_New("col2", EagleDbColumnTypeInteger));
    
    /* put some data in it */
    db->td = EagleDbTableData_New(table);
    
    for(i = 0; i < 10; ++i) {
        /* create a record */
        EagleDbTuple *tuple = EagleDbTuple_New(table);
        EagleDbTuple_setInt(tuple, 0, i);
        EagleDbTuple_setInt(tuple, 1, i * 2);
        
        /* put record in */
        EagleDbTableData_insert(db->td, tuple);
    }
    
    return db;
}

void EagleDbInstance_PrintResults(EaglePlan *plan)
{
    int i, j, k, *widths;
    EaglePage **pages;
    int totalRecords = 0;
    
    /* calculate the widths of the fields */
    widths = (int*) calloc((size_t) plan->resultFields, sizeof(int));
    for(i = 0; i < plan->resultFields; ++i) {
        widths[i] = (int) strlen(plan->result[i]->name);
    }
    
    /* heading */
    printf("\n");
    for(i = 0; i < plan->resultFields; ++i) {
        if(i > 0) {
            printf("|");
        }
        printf(" %s ", plan->result[i]->name);
    }
    printf("\n");
    
    for(i = 0; i < plan->resultFields; ++i) {
        if(i > 0) {
            printf("+");
        }
        for(j = 0; j < widths[i] + 2; ++j) {
            printf("-");
        }
    }
    printf("\n");
    
    /* render out */
    pages = (EaglePage**) calloc((size_t) plan->resultFields, sizeof(EaglePage*));
    while(1) {
        int finished = 0;
        for(i = 0; i < plan->resultFields; ++i) {
            EaglePage *page = EaglePageProvider_nextPage(plan->result[i]);
            if(NULL == page) {
                finished = 1;
                break;
            }
            pages[i] = page;
        }
        
        if(finished == 0) {
            for(j = 0; j < pages[0]->count; ++j) {
                for(k = 0; k < plan->resultFields; ++k) {
                    if(k > 0) {
                        printf("|");
                    }
                    printf(" %*d ", widths[k], pages[k]->data[j]);
                }
                printf("\n");
                ++totalRecords;
            }
            
            for(k = 0; k < plan->resultFields; ++k) {
                EaglePage_Delete(pages[k]);
            }
        }
        else {
            break;
        }
    }
    
    printf("\n%d record%s, %.3f seconds\n\n", totalRecords, (totalRecords == 1 ? "" : "s"), EaglePlan_getExecutionSeconds(plan));
}

void EagleDbInstance_executeSelect(EagleDbInstance *db)
{
    EaglePlan *plan;
    
    EagleDbSqlSelect *select = (EagleDbSqlSelect*) yyparse_ast;
    plan = EagleDbSqlSelect_parse(select, db);
    /*printf("%s\n", EaglePlan_toString(plan));*/
    
    /* catch compilation error */
    if(EagleTrue == EaglePlan_isError(plan)) {
        printf("Error: %s\n", plan->errorMessage);
        EaglePlan_Delete(plan);
    }
    else {
        /* execute */
        EagleInstance *eagle = EagleInstance_New(1);
        EagleInstance_addPlan(eagle, plan);
        EagleInstance_run(eagle);
        
        /* print results */
        EagleDbInstance_PrintResults(plan);
        
        EagleInstance_Delete(eagle);
    }
}

void EagleDbInstance_execute(EagleDbInstance *db, char *sql)
{
    /* parse sql */
    yylex_init();
    yy_scan_string(sql);
    yyparse();
    
    /* check for errors */
    if(NULL != yyerrors_last()) {
        printf("Error: %s\n", yyerrors_last());
    }
    else {
        /* select */
        EagleDbInstance_executeSelect(db);
    }
    
    /* clean up */
    EagleDbSqlSelect_Delete(yyparse_ast);
    yylex_free();
}

void EagleDbInstance_Delete(EagleDbInstance *db)
{
    free(db);
}
