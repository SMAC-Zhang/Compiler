#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "slp.h"
#include "y.tab.h"

A_exp root;

extern int yyparse();

int main(int argc, const char * argv[]) {
    yyparse();
    return 0;
}