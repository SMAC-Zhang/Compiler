#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "slp.h"
#include "y.tab.h"
#include "compile_slp.h"

A_stm root_stm;

extern int yyparse();

void generate_main_ll(FILE *stream, A_stm stm) {
    
//declare extern func + function head
    fprintf(stream, "declare void @putint(i32) #1\n");
    fprintf(stream, "declare void @putch(i32) #1\n");
    fprintf(stream, "define dso_local i32 @main() #0 {\n");

    compile_stmt(stream, stm);
    fprintf(stream, "    ret i32 0;\n }\n");

    return;
}

int main(int argc, const char * argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./a.out *.in *.out\n");
    }

    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w", stdout);

    yyparse();
    generate_main_ll(stdout, root_stm);
    return 0;
}