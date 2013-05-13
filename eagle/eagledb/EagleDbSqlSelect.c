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
    
    return select;
}

void EagleDbSqlSelect_Delete(EagleDbSqlSelect *select)
{
    if(NULL == select) {
        return;
    }
    
    EagleMemory_Free(select->tableName);
    EagleMemory_Free(select);
}

void EagleDbSqlSelect_DeleteRecursive(EagleDbSqlSelect *select)
{
    if(NULL == select) {
        return;
    }
    
    EagleLinkedList_DeleteWithItems(select->selectExpressions);
    EagleDbSqlExpression_DeleteRecursive(select->whereExpression);
    EagleMemory_Free(select->tableName);
    EagleMemory_Free(select);
}

int EagleDbSqlSelect_getFieldCount(EagleDbSqlSelect *select)
{
    return EagleLinkedList_length(select->selectExpressions);
}

int EagleDbSqlSelect_getExpressionsCount(EagleDbSqlSelect *select)
{
    int exprs = EagleDbSqlSelect_getFieldCount(select);
    if(NULL != select->whereExpression) {
        ++exprs;
    }
    return exprs;
}

EaglePlan* EagleDbSqlSelect_parse(EagleDbSqlSelect *select, struct EagleDbInstance *db)
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
    plan = EaglePlan_New(db->pageSize, db->cores);
    
    /* get data */
    td = EagleDbInstance_getTable(db, select->tableName);
    if(NULL == td) {
        EaglePlan_setError(plan, EaglePlanErrorNoSuchTable, select->tableName);
        return plan;
    }
    for(i = 0; i < EagleLinkedList_length(td->table->columns); ++i) {
        EaglePlanBufferProvider *bp;
        
        EaglePageProvider_reset(td->providers[i]);
        bp = EaglePlanBufferProvider_NewWithProvider(i, td->providers[i], EagleFalse);
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
        expr[expri] = EagleLinkedList_get(select->selectExpressions, expri);
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
    EagleLinkedListItem *item = NULL;
    char *str = (char*) EagleMemory_Allocate("EagleDbSqlSelect_toString.1", 1024);
    if(NULL == str) {
        return NULL;
    }
    
    str[0] = '\0';
    strcat_safe(str, "SELECT ");
    
    for(item = EagleLinkedList_begin(select->selectExpressions); item; item = item->next) {
        char *s;
        if(item != EagleLinkedList_begin(select->selectExpressions)) {
            strcat_safe(str, ", ");
        }
        
        s = EagleDbSqlExpression_toString((EagleDbSqlExpression*) item->obj);
        strcat_safe(str, s);
        EagleMemory_Free(s);
    }
    
    strcat_safe(str, " FROM ");
    strcat_safe(str, select->tableName);
    
    if(NULL != select->whereExpression) {
        char *s;
        
        strcat_safe(str, " WHERE ");
        s = EagleDbSqlExpression_toString(select->whereExpression);
        strcat_safe(str, s);
        EagleMemory_Free(s);
    }
    
    return str;
}
