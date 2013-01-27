#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EagleDbSqlSelect.h"
#include "EagleMemory.h"
#include "EagleUtils.h"
#include "EagleDbSqlExpressionType.h"
#include "EagleDbTableData.h"
#include "EagleDbInstance.h"

EagleDbSqlSelect* EagleDbSqlSelect_New(void)
{
    EagleDbSqlSelect *select = (EagleDbSqlSelect*) EagleMemory_Allocate("EagleDbSqlSelect_New.1", sizeof(EagleDbSqlSelect));
    if(NULL == select) {
        return NULL;
    }
    
    select->expressionType = EagleDbSqlExpressionTypeSelect;
    select->tableName = NULL;
    select->whereExpression = NULL;
    select->selectExpressions = NULL;
    select->usedSelectExpressions = 0;
    select->allocatedSelectExpressions = 0;
    
    return select;
}

void EagleDbSqlSelect_Delete(EagleDbSqlSelect *select, EagleBoolean recursive)
{
    if(NULL == select) {
        return;
    }
    
    if(EagleTrue == recursive) {
        int i;
        for(i = 0; i < select->usedSelectExpressions; ++i) {
            EagleDbSqlExpression_Delete(select->selectExpressions[i], recursive);
        }
        EagleMemory_Free(select->selectExpressions);
        
        EagleDbSqlExpression_Delete(select->whereExpression, recursive);
    }
    
    EagleMemory_Free(select->tableName);
    EagleMemory_Free(select);
}

int EagleDbSqlSelect_getFieldCount(EagleDbSqlSelect *select)
{
    return select->usedSelectExpressions;
}

int EagleDbSqlSelect_getExpressionsCount(EagleDbSqlSelect *select)
{
    int exprs = EagleDbSqlSelect_getFieldCount(select);
    if(NULL != select->whereExpression) {
        ++exprs;
    }
    return exprs;
}

EaglePlan* EagleDbSqlSelect_parse(EagleDbSqlSelect *select, struct EagleDbInstance_ *db)
{
    int i;
    int exprCount, whereExpressionId = -1, expri = 0;
    EaglePlan *plan;
    EagleDbSqlExpression **expr;
    EagleDbTableData *td;
    
    if(NULL == select) {
        return NULL;
    }
    if(NULL == db) {
        return NULL;
    }
    
    /* create the plan skeleton */
    plan = EaglePlan_New(db->pageSize);
    
    /* get data */
    td = EagleDbInstance_getTable(db, select->tableName);
    if(NULL == td) {
        EaglePlan_setError(plan, EaglePlanErrorNoSuchTable, select->tableName);
        return plan;
    }
    for(i = 0; i < td->table->usedColumns; ++i) {
        EaglePlanBufferProvider *bp;
        
        EaglePageProvider_reset(td->providers[i]);
        bp = EaglePlanBufferProvider_New(i, td->providers[i], EagleFalse);
        EaglePlan_addBufferProvider(plan, bp, EagleTrue);
    }
    
    /* merge expressions */
    exprCount = EagleDbSqlSelect_getExpressionsCount(select);
    expr = (EagleDbSqlExpression**) EagleMemory_MultiAllocate("EagleDbSqlSelect_parse.2", sizeof(EagleDbSqlExpression*), exprCount);
    if(NULL == expr) {
        EaglePlan_Delete(plan);
        return NULL;
    }
    
    for(expri = 0; expri < EagleDbSqlSelect_getFieldCount(select); ++expri) {
        expr[expri] = select->selectExpressions[expri];
    }
    if(NULL != select->whereExpression) {
        whereExpressionId = expri;
        expr[whereExpressionId] = select->whereExpression;
    }
    
    /* compile plan */
    EagleDbSqlExpression_CompilePlan(expr, exprCount, whereExpressionId, plan);
    EagleMemory_Free(expr);
    
    return plan;
}

char* EagleDbSqlSelect_toString(EagleDbSqlSelect *select)
{
    return strdup("SELECT");
}
