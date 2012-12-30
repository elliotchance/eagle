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
    EagleDbTable_addColumn(table, EagleDbColumn_New("col1", EagleDataTypeInteger));
    EagleDbTable_addColumn(table, EagleDbColumn_New("col2", EagleDataTypeText));
    
    /* put some data in it */
    db->td = EagleDbTableData_New(table);
    
    for(i = 0; i < 10; ++i) {
        /* create a record */
        EagleDbTuple *tuple = EagleDbTuple_New(table);
        EagleDbTuple_setInt(tuple, 0, i);
        EagleDbTuple_setText(tuple, 1, "hello");
        
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
            if(NULL == pages || NULL == pages[0]) {
                break;
            }
            for(j = 0; j < pages[0]->count; ++j) {
                for(k = 0; k < plan->resultFields; ++k) {
                    if(k > 0) {
                        printf("|");
                    }
                    
                    switch(pages[k]->type) {
                            
                        case EagleDataTypeUnknown:
                            printf(" %*s ", widths[k], "?");
                            break;
                            
                        case EagleDataTypeInteger:
                        {
                            int *d = (int*) pages[k]->data;
                            printf(" %*d ", widths[k], d[j]);
                            break;
                        }
                            
                        case EagleDataTypeText:
                        {
                            char **d = (char**) pages[k]->data;
                            printf(" %*s ", widths[k], d[j]);
                            break;
                        }
                            
                    }
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
    free(pages);
    free(widths);
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
    
    EagleDbSqlSelect_Delete(select);
}

void EagleDbInstance_executeCreateTable(EagleDbInstance *db)
{
    EagleDbTable *table = (EagleDbTable*) yyparse_ast;
    printf("Table '%s' created.\n\n", table->name);
    
    EagleDbTable_Delete(table);
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
        switch(yystatementtype) {
            case EagleDbSqlStatementTypeNone:
                /* lets not consider this an error and ignore it */
                break;
                
            case EagleDbSqlStatementTypeSelect:
                EagleDbInstance_executeSelect(db);
                break;
                
            case EagleDbSqlStatementTypeCreateTable:
                EagleDbInstance_executeCreateTable(db);
                break;
        }
    }
    
    /* clean up */
    yylex_free();
}

void EagleDbInstance_Delete(EagleDbInstance *db)
{
    free(db);
}
