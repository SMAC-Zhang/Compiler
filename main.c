#include <string.h>

#include "printast.h"
#include "y.tab.h"
#include "type_check.h"
#include "translate.h"
#include "pr_tree_readable.h"

A_prog root;
A_prog prog1();

int main(int argc, char* argv[]) {

    yyparse(); 
    // printA_Prog(stdout, root);
    check_Prog(stdout, root);
    printFuncDeclList(stdout, translate_Prog(stdout, root));
    return 0;
}