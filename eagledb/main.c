#include <stdio.h>

extern int yyparse();
extern int yy_scan_string(const char *);

int main(int argc, const char * argv[])
{
    const char *sql = "15 + 7";
    
    yy_scan_string(sql);
    
    if(yyparse() == 0) {
        fprintf(stderr, "Successful parsing.\n");
    }
    else {
        fprintf(stderr, "error found.\n");
    }
    
    // clean
    //yylex_destroy();

    return 0;
}
