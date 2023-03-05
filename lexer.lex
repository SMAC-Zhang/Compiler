%{

#include "util.h"
#include "slp.h"
#include "y.tab.h"

int c;

%}

%%

" " ;
"\r\n";
"\t";
"\r";
"\n";
"+" { yylval.token = OP_PLUS; return OP_PLUS; }
"*" { yylval.token = OP_MULTIPLY; return OP_MULTIPLY; }
"-" { yylval.token = OP_MINUS; return OP_MINUS; }
"/" { yylval.token = OP_DIV; return OP_DIV; }
":=" { yylval.token = ASSIGN; return ASSIGN; }
"print" { yylval.token = PRINT; return PRINT; }
"("|")" {
    c = yytext[0];
    return(c);
}
","|";" {
    c = yytext[0];
    return(c);
}
[a-z_A-Z][a-z_A-Z0-9]* { 
    yylval.id = String(yytext);
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
    for(int i = 0; i < len; i++)
        ret = ret * 10 + (s[i] - '0');
    return ret;
}