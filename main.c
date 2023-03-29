#include <string.h>

#include "printast.h"
#include "y.tab.h"
#include "type_check.h"

A_prog root;
A_prog prog1();

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        fprintf(stderr, "main argc error!\n");
    }

    if (strcmp(argv[1], "c") == 0) {
        check_Prog(stdout, prog1());
    } else if (strcmp(argv[1], "fmj") == 0) {
        yyparse(); 
        // printA_Prog(stdout, root);
        check_Prog(stdout, root);
    } else {
        fprintf(stderr, "main usage error!\n");
    }
    return 0;
}