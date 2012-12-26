#include <stdlib.h>
#include <stdio.h>

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

void EagleDbInstance_executeSelect(EagleDbInstance *db)
{
    int exprs = 1;
    int pageSize = 10;
    int i;
    EaglePlan *plan;
    EagleDbSqlExpression **expr;
    
    /* the providers will contain the result */
    EaglePageProvider **providers = (EaglePageProvider**) calloc((size_t) exprs, sizeof(EaglePageProvider*));
    providers[0] = EaglePageProvider_CreateFromIntStream(pageSize, "answer");
    
    /* create the plan skeleton */
    plan = EaglePlan_New(pageSize);
    
    /* get data */
    for(i = 0; i < db->td->table->usedColumns; ++i) {
        EaglePlanBufferProvider *bp;
        
        EaglePageProvider_reset(db->td->providers[i]);
        bp = EaglePlanBufferProvider_New(i, db->td->providers[i], EagleFalse);
        EaglePlan_addBufferProvider(plan, bp);
    }
    
    /* compile plan */
    expr = (EagleDbSqlExpression**) calloc((size_t) 1, sizeof(EagleDbSqlExpression*));
    expr[0] = yyparse_ast;
    EagleDbSqlExpression_CompilePlan(expr, exprs, -1, providers, plan);
    
    /* catch compilation error */
    if(EagleTrue == EaglePlan_isError(plan)) {
        printf("Error: %s\n", plan->errorMessage);
        EaglePlan_Delete(plan);
    }
    else {
        /* execute */
        EaglePage *page = NULL;
        EagleInstance *eagle = EagleInstance_New(1);
        EagleInstance_addPlan(eagle, plan);
        EagleInstance_run(eagle);
        
        /* print results */
        page = EaglePageProvider_nextPage(providers[0]);
        
        if(NULL != page) {
            for(i = 0; i < page->count; ++i) {
                printf("%d\n", page->data[i]);
            }
            EaglePage_Delete(page);
        }
        
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
