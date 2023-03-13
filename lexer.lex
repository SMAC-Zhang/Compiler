%{

#include "util.h"
#include "fdmjast.h"
#include "y.tab.h"

int c;
int line = 1;
int pos = 0;

%start COMMENT1 COMMENT2

%}

%%

"//" { BEGIN COMMENT1; }
"/*" { BEGIN COMMENT2; }
<COMMENT1> "\n" { line++; pos = 0; BEGIN INITIAL; }
<COMMENT1> "\t" { pos += 4; }
<COMMENT1> . { pos++; }
<COMMENT2> "\n" { line++; pos = 0;}
<COMMENT2> "\t" { pos += 4; }
<COMMENT2> . { pos++; }
<COMMENT2> "*/" { pos++; BEGIN INITIAL; }

<INITIAL> " " { pos++; }
<INITIAL> "\t" { pos += 4; };
<INITIAL> "\n" { line++; pos = 0;};

<INITIAL> "+" { pos++; yylval.token = A_plus; return OP_PLUS; }
<INITIAL> "*" { pos++; yylval.token = A_times; return OP_MULTIPLY; }
<INITIAL> "-" { pos++; yylval.token = A_minus; return OP_MINUS; }
<INITIAL> "/" { pos++; yylval.token = A_div; return OP_DIV; }
<INITIAL> "public" { pos += 6; yylval.token = PUBLIC; return PUBLIC; }
<INITIAL> "int" { pos += 3; yylval.token = INT; return INT; }
<INITIAL> "main" { pos += 4; yylval.token = MAIN; return MAIN; }
<INITIAL> "putint" { pos+= 6; yylval.token = PUTINT; return PUTINT; }
<INITIAL> "putch" { pos += 5; yylval.token = PUTCH; return PUTCH; }

<INITIAL> [1-9][0-9]* {
    pos += yyleng;
    yylval.token = calc(yytext, yyleng);
    return INT_CONST;
}
<INITIAL> 0 {
    pos++;
    yylval.token = 0;
    return INT_CONST;
}

<INITIAL> [a-z_A-Z][a-z_A-Z0-9]* { 
    pos += yyleng;
    yylval.id = String(yytext);
    return IDENTIFIER;
}

<INITIAL> "("|")"|"="|";"|"{"|"}" {
    pos++;
    c = yytext[0];
    return(c);
}

<INITIAL> . {
    pos++;
    printf("Illegal input \"%c\" found at line =%d, pos=%d\n", yytext[0], line, pos);
}
%%


int calc(char *s, int len) {
    int ret = 0;
    for(int i = 0; i < len; i++)
        ret = ret * 10 + (s[i] - '0');
    return ret;
}