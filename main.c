#include "printast.h"
#include "y.tab.h"

A_prog root;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: ./main *.out");
    }

    FILE* stream = fopen(argv[0], "w");

    yyparse();
    printA_Prog(stream, root);
    return 0;
}