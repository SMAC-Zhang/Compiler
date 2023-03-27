#include "printast.h"
#include "y.tab.h"
#include "type_check.h"

A_prog root;
A_prog prog1();

int main(int argc, char* argv[]) {
    
    yyparse();
    check_Prog(stdout, root);
    return 0;
}