%{

#include "util.h"
#include "slp.h"
#include "y.tab.h"

int c;

%}

%%

" " ;
"+" { yylval.token = OP_PLUS; return OP_PLUS; }
"*" { yylval.token = OP_MULTIPLY; return OP_MULTIPLY; }
"("|")" {
    c = yytext[0];
    return(c);
}
[a-z_A-Z][a-z_A-Z0-9]* { 
    yylval.expr = A_IdExp(String(yytext));
    return IDENTIFIER;
}
[1-9][0-9]* {
    yylval.token = calc(yytext, yyleng);
    return NUMBER;
}
0 {
    yylval.token = 0;
    return NUMBER;
}
. {
    printf("Illegal input \"%c\"\n", yytext[0]);
}
%%


int calc(char *s, int len) {
    int ret = 0;
    for(int i = 1; i < len; i++)
        ret = ret * 10 + (s[i] - '0');
    return ret;
}