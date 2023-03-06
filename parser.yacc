%{
#include<stdio.h>
#include "slp.h"

extern A_stm root_stm;

extern int yylex();
extern void yyerror(char*);
extern int  yywrap();

// extern int yydebug = 1;


%}

%union
{
    int token; // 例：数字类
    string id;
    A_exp expr; // 例：自定义的类
    A_stm stm; // statement
    A_expList explist;
}

// token的类
// %token <name in union> token_name_1 token_name_2
%token <token> OP_PLUS OP_MULTIPLY OP_MINUS OP_DIV END
%token <token> ASSIGN PRINT
%token <token> NUMBER
%token <id> IDENTIFIER

// 非终结符的类
%type <expr> EXPR
%type <stm> STM
%type <stm> CSTM
%type <explist> EXPLIST

%start CSTM

%%

STM:  IDENTIFIER ASSIGN EXPR
      {
          root_stm = A_AssignStm($1, $3);
          $$ = A_AssignStm($1, $3);
      }
      |
      PRINT '(' EXPLIST ')'
      {
          root_stm = A_PrintStm($3);
          $$ = A_PrintStm($3);
      }

CSTM: STM ';' CSTM
      {
          root_stm = A_CompoundStm($1, $3);
          $$ = A_CompoundStm($1, $3);
      }
      |
      {
          root_stm = A_CompoundStm(NULL, NULL);
          $$ = A_CompoundStm(NULL, NULL);
      }

EXPLIST: 
      EXPR
      {
          $$ = A_LastExpList($1);
      }
      |
      EXPR ',' EXPLIST 
      {
          $$ = A_PairExpList($1, $3);
      }

EXPR: NUMBER
      {
          $$ = A_NumExp($1);
      } 
      | 
      IDENTIFIER 
      {
          $$ = A_IdExp($1);
      }
      |
      '(' EXPR OP_PLUS EXPR ')' 
      {
          $$ = A_OpExp($2, A_plus, $4);
      } 
      |
      '(' EXPR OP_MULTIPLY EXPR ')' 
      {
          $$ = A_OpExp($2, A_times, $4);
      }
      |
      '(' EXPR OP_MINUS EXPR ')'
      {
          $$ = A_OpExp($2, A_minus, $4);
      }
      |
      '(' EXPR OP_DIV EXPR ')'
      {
          $$ = A_OpExp($2, A_div, $4);
      }
      |
	  '(' STM ',' EXPR ')'
      {
		  $$ = A_EseqExp($2, $4);
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
