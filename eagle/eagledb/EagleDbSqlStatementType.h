#ifndef eagle_EagleDbSqlStatementType_h
#define eagle_EagleDbSqlStatementType_h

typedef enum {
    
    /**
     Indicated a blank SQL statement. This may or may not be an error depending on the context.
     */
    EagleDbSqlStatementTypeNone = 0,
    
    /**
     SELECT statement.
     @see EagleDbSqlSelect
     */
    EagleDbSqlStatementTypeSelect = 1,
    
    /**
     CREATE TABLE statement.
     @see EagleDbTable
     */
    EagleDbSqlStatementTypeCreateTable = 2,
    
    /**
     INSERT statement.
     @see EagleDbSqlInsert
     */
    EagleDbSqlStatementTypeInsert = 3
    
} EagleDbSqlStatementType;

#endif
