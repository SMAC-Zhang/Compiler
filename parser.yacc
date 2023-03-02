%{
#include<stdio.h>
#include "slp.h"

extern A_exp root;

extern int yylex();
extern void yyerror(char*);
extern int  yywrap();

// extern int yydebug = 1;


%}

%union
{
    int token; // 例：数字类
    A_exp expr; // 例：自定义的类
}

// token的类
// %token <name in union> token_name_1 token_name_2
%token <token> OP_PLUS OP_MULTIPLY
%token <token> NUMBER
%token <expr> IDENTIFIER

// 非终结符的类
%type <expr> EXPR

%%

EXPR: NUMBER
      {
          $$ = A_NumExp($1);
      } 
      | 
      IDENTIFIER 
      {
          $$ = $1;
      }
      |
      '(' EXPR OP_PLUS EXPR ')' 
      {
          root = A_OpExp($2, A_plus, $4);
          $$ = A_OpExp($2, A_plus, $4);
      } 
      | 
      '(' EXPR OP_MULTIPLY EXPR ')' 
      {
          root = A_OpExp($2, A_times, $4);
          $$ = A_OpExp($2, A_times, $4);
      }
%%

void yyerror(s)
char *s;
{
  fprintf(stderr, "%s\n",s);
}

int yywrap()
{
  return(1);
}
