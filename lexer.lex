%{

#include "util.h"
#include "fdmjast.h"
#include "y.tab.h"

extern int calc(char*, int);
int c;
int line = 1;
int pos = 0;

%}
%start COMMENT1 COMMENT2

%%

"//" { BEGIN COMMENT1; }
"/*" { BEGIN COMMENT2; }
<COMMENT1>"\n" { line++; pos = 0; BEGIN INITIAL; }
<COMMENT1>"\t" { pos += 4; }
<COMMENT1>. { pos++; }
<COMMENT2>"\n" { line++; pos = 0;}
<COMMENT2>"\t" { pos += 4; }
<COMMENT2>. { pos++; }
<COMMENT2>"*/" { pos++; BEGIN INITIAL; }

<INITIAL>" " { pos++; }
<INITIAL>"\t" { pos += 4; }
<INITIAL>"\n" { line++; pos = 0; }

<INITIAL>"+" { yylval.token = A_Pos(line, pos); pos += yyleng; return OP_PLUS; }
<INITIAL>"*" { yylval.token = A_Pos(line, pos); pos += yyleng; return OP_MULTIPLY; }
<INITIAL>"-" { yylval.token = A_Pos(line, pos); pos += yyleng; return OP_MINUS; }
<INITIAL>"/" { yylval.token = A_Pos(line, pos); pos += yyleng; return OP_DIV; }
<INITIAL>"<=" { yylval.token = A_Pos(line, pos); pos += yyleng; return LE; }
<INITIAL>">=" { yylval.token = A_Pos(line, pos); pos += yyleng; return GE; }
<INITIAL>"<" { yylval.token = A_Pos(line, pos); pos += yyleng; return LESS; }
<INITIAL>">" { yylval.token = A_Pos(line, pos); pos += yyleng; return GREATER; }
<INITIAL>"&&" { yylval.token = A_Pos(line, pos); pos += yyleng; return AND; }
<INITIAL>"||" { yylval.token = A_Pos(line, pos); pos += yyleng; return OR; }
<INITIAL>"!" { yylval.token = A_Pos(line, pos); pos += yyleng; return NOT; }

<INITIAL>"public" { yylval.token = A_Pos(line, pos); pos += yyleng; return PUBLIC; }
<INITIAL>"int" { yylval.token = A_Pos(line, pos); pos += yyleng; return INT; }
<INITIAL>"main" { yylval.token = A_Pos(line, pos); pos += yyleng; return MAIN; }
<INITIAL>"putint" { yylval.token = A_Pos(line, pos); pos += yyleng; return PUTINT; }
<INITIAL>"putch" { yylval.token = A_Pos(line, pos); pos += yyleng; return PUTCH; }
<INITIAL>"class" { yylval.token = A_Pos(line, pos); pos += yyleng; return CLASS; }
<INITIAL>"extends" { yylval.token = A_Pos(line, pos); pos += yyleng; return EXTENDS; }
<INITIAL>"if" { yylval.token = A_Pos(line, pos); pos += yyleng; return IF; }
<INITIAL>"else" { yylval.token = A_Pos(line, pos); pos += yyleng; return ELSE; }
<INITIAL>"while" { yylval.token = A_Pos(line, pos); pos += yyleng; return WHILE; }
<INITIAL>"continue" { yylval.token = A_Pos(line, pos); pos += yyleng; return CONTINUE; }
<INITIAL>"break" { yylval.token = A_Pos(line, pos); pos += yyleng; return BREAK; }
<INITIAL>"return" { yylval.token = A_Pos(line, pos); pos += yyleng; return RETURN; }

<INITIAL>"putint" { yylval.token = A_Pos(line, pos); pos += yyleng; return PUTINT; }
<INITIAL>"putch" { yylval.token = A_Pos(line, pos); pos += yyleng; return PUTCH; }
<INITIAL>"putarray" { yylval.token = A_Pos(line, pos); pos += yyleng; return PUTARRAT; }
<INITIAL>"getint" { yylval.token = A_Pos(line, pos); pos += yyleng; return GETINT; }
<INITIAL>"getch" { yylval.token = A_Pos(line, pos); pos += yyleng; return GETCH; }
<INITIAL>"getarray" { yylval.token = A_Pos(line, pos); pos += yyleng; return GETARRAY; }
<INITIAL>"starttime" { yylval.token = A_Pos(line, pos); pos += yyleng; return STARTTIME; }
<INITIAL>"stoptime" { yylval.token = A_Pos(line, pos); pos += yyleng; return STOPTIME; }

<INITIAL>"true" { yylval.token = A_Pos(line, pos); pos += yyleng; return TRUE; }
<INITIAL>"false" { yylval.token = A_Pos(line, pos); pos += yyleng; return FALSE; }
<INITIAL>"length" { yylval.token = A_Pos(line, pos); pos += yyleng; return LENGTH; }
<INITIAL>"this" { yylval.token = A_Pos(line, pos); pos += yyleng; return THIS; }
<INITIAL>"new" { yylval.token = A_Pos(line, pos); pos += yyleng; return NEW; }

<INITIAL>[1-9][0-9]* {
    yylval.exp = A_NumConst(A_Pos(line, pos), calc(yytext, yyleng));
    pos += yyleng;
    return INT_CONST;
}
<INITIAL>"-"[1-9][0-9]* {
    yylval.exp = A_NumConst(A_Pos(line, pos), -calc(yytext + 1, yyleng - 1));
    pos += yyleng;
    return INT_CONST;
}
<INITIAL>0 {
    yylval.exp = A_NumConst(A_Pos(line, pos), 0);
    pos++;
    return INT_CONST;
}

<INITIAL>[a-z_A-Z][a-z_A-Z0-9]* { 
    yylval.id = String(yytext);
    pos += yyleng;
    return IDENTIFIER;
}

<INITIAL>"{" { yylval.token = A_Pos(line, pos); pos++; return BRACE; }
<INITIAL>"(" { yylval.token = A_Pos(line, pos); pos++; return PARENTHESIS; }
<INITIAL>")"|"="|','|";"|"}"|"["|"]"|"." {
    pos++;
    c = yytext[0];
    return(c);
}

<INITIAL>. {
    pos++;
    printf("Illegal input \"%c\" found at line = %d, pos = %d\n", yytext[0], line, pos);
}
%%


int calc(char *s, int len) {
    int ret = 0;
    for(int i = 0; i < len; i++) {
        if (s[i] - '0' >= 10) {
            printf("ERROR in calc!\n");
        }
        ret = ret * 10 + (s[i] - '0');
    }
    return ret;
}