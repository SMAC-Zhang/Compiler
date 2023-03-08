#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "slp.h"
#include "y.tab.h"
#include "interp_slp.h"

A_stm root_stm;

extern int yyparse();

int main(int argc, const char * argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./a.out *.in *.out\n");
    }

    freopen(argv[1], "r", stdin);
    FILE *stream = fopen(argv[2], "w");

    yyparse();
    interp_stmt(stream, root_stm);
    return 0;
}