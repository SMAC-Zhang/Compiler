#include "printast.h"
#include "y.tab.h"

A_prog root;

int main() {
    yyparse();
    printA_Prog(stdout, root);
    return 0;
}