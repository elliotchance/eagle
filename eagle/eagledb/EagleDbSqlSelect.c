#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "EagleDbSqlSelect.h"

/**
 Create a new EagleDbSqlSelect.
 */
EagleDbSqlSelect* EagleDbSqlSelect_New(void)
{
    EagleDbSqlSelect *select = (EagleDbSqlSelect*) malloc(sizeof(EagleDbSqlSelect));
    
    select->expressionType = EagleDbSqlExpressionTypeSelect;
    select->tableName = NULL;
    select->whereExpression = NULL;
    select->selectExpressions = NULL;
    select->usedSelectExpressions = 0;
    select->allocatedSelectExpressions = 0;
    
    return select;
}

void EagleDbSqlSelect_Delete(EagleDbSqlSelect *select)
{
    int i;
    
    if(NULL == select) {
        return;
    }
    if(select->tableName) {
        free(select->tableName);
    }
    EagleDbSqlExpression_Delete(select->whereExpression);
    
    for(i = 0; i < select->usedSelectExpressions; ++i) {
        EagleDbSqlExpression_Delete(select->selectExpressions[i]);
    }
    free(select->selectExpressions);
    
    free(select);
}

/**
 Return the number of expressions after the SELECT clause.
 */
int EagleDbSqlSelect_getFieldCount(EagleDbSqlSelect *select)
{
    return select->usedSelectExpressions;
}

/**
 Returns the total amount of expressions for an entire SELECT statements (containing all its clauses)
 */
int EagleDbSqlSelect_getExpressionsCount(EagleDbSqlSelect *select)
{
    int exprs = EagleDbSqlSelect_getFieldCount(select);
    if(NULL != select->whereExpression) {
        ++exprs;
    }
    return exprs;
}

EaglePlan* EagleDbSqlSelect_parse(EagleDbSqlSelect *select, EagleDbInstance *db)
{
    int pageSize = 10;
    int i;
    int exprCount, whereExpressionId = -1, expri = 0;
    EaglePlan *plan;
    EagleDbSqlExpression **expr;
    
    /* create the plan skeleton */
    plan = EaglePlan_New(pageSize);
    
    /* the providers will contain the result */
    plan->resultFields = EagleDbSqlSelect_getFieldCount(select);
    plan->result = (EaglePageProvider**) calloc((size_t) plan->resultFields, sizeof(EaglePageProvider*));
    for(i = 0; i < plan->resultFields; ++i) {
        plan->result[i] = EaglePageProvider_CreateFromIntStream(pageSize, EagleDbSqlExpression_toString(select->selectExpressions[i]));
    }
    
    /* get data */
    for(i = 0; i < db->td->table->usedColumns; ++i) {
        EaglePlanBufferProvider *bp;
        
        EaglePageProvider_reset(db->td->providers[i]);
        bp = EaglePlanBufferProvider_New(i, db->td->providers[i], EagleFalse);
        EaglePlan_addBufferProvider(plan, bp);
    }
    
    /* merge expressions */
    exprCount = EagleDbSqlSelect_getExpressionsCount(select);
    expr = (EagleDbSqlExpression**) calloc((size_t) exprCount, sizeof(EagleDbSqlExpression*));
    for(expri = 0; expri < exprCount; ++expri) {
        expr[expri] = select->selectExpressions[expri];
    }
    if(NULL != select->whereExpression) {
        whereExpressionId = expri;
        expr[whereExpressionId] = select->whereExpression;
    }
    
    /* compile plan */
    EagleDbSqlExpression_CompilePlan(expr, exprCount, whereExpressionId, plan);
    
    return plan;
}

char* EagleDbSqlSelect_toString(EagleDbSqlSelect *select)
{
    return strdup("SELECT");
}
