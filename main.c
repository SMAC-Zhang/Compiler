#include "printast.h"
#include "y.tab.h"

A_prog root;

int main(int argc, char* argv[]) {
    
    yyparse();
    printA_Prog(stdout, root);
    return 0;
}