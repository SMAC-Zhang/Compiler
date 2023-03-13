%{
#include <stdio.h>
#include <string.h>
#include "fdmjast.h"
#include "util.h"

extern A_prog root; 
extern int line, pos;

extern int yylex();
extern void yyerror(char*);
extern int  yywrap();
extern int len_id();
extern int len_int();

// extern int yydebug = 1;

%}

%union
{
    int token;
    string id;
    A_prog prog;
    A_type type;
    A_mainMethod main;
    A_stm stm;
	A_stmList stmlist;
    A_exp exp;
}

// 运算符
%token <token> OP_PLUS OP_MULTIPLY OP_MINUS OP_DIV UMINUS
// 关键字
%token <token> PUBLIC INT MAIN PUTCH PUTINT
// 数字
%token <token> INT_CONST
// 标识符
%token <id> IDENTIFIER

// 非终结符的类
%type <prog> PROG
%type <main> MAIN_METHOD
%type <stm> STM
%type <stmlist> STM_LIST
%type <exp> EXP

%start PROG
%left OP_PLUS OP_MINUS
%left OP_MULTIPLY OP_DIV
%left UMINUS

%%
PROG: MAIN_METHOD
	{
		root = A_Prog($1->pos, $1, NULL);
		$$ = A_Prog($1->pos, $1, NULL);
	}

MAIN_METHOD: PUBLIC INT MAIN '(' ')' '{' STM_LIST '}'
	{
		$$ = A_MainMethod(A_Pos($7->head->pos->line, $7->head->pos->pos - 19), NULL, $7);
	}

STM: 
	IDENTIFIER '=' EXP ';' 
	{
		A_exp e = A_IdExp(A_Pos($3->pos->line, $3->pos->pos - len_id($1) - 2), $1);
		$$ = A_AssignStm(e->pos, e, NULL, $3);
	}
	|
	PUTINT '(' EXP ')' ';'
	{
		$$ = A_Putint(A_Pos($3->pos->line, $3->pos->pos - 7), $3);
	}
	|
	PUTCH '(' EXP ')' ';'
	{
		$$ = A_Putch(A_Pos($3->pos->line, $3->pos->pos - 6), $3);
	}

STM_LIST:
	STM STM_LIST
	{
		$$ = A_StmList($1, $2);
	}
	|
	{

	}

EXP:
	EXP OP_PLUS EXP
	{
		$$ = A_OpExp($1->pos, $1, A_plus, $3);
	}
	|
	EXP OP_MINUS EXP
	{
		$$ = A_OpExp($1->pos, $1, A_minus, $3);
	}
	|
	OP_MINUS EXP %prec UMINUS
	{
		$$ = A_MinusExp(A_Pos($2->pos->line, $2->pos->pos - 1), $2);
	}
	|
	EXP OP_MULTIPLY EXP
	{
		$$ = A_OpExp($1->pos, $1, A_times, $3);
	}
	|
	EXP OP_DIV EXP
	{
		$$ = A_OpExp($1->pos, $1, A_div, $3);
	}
	|
	INT_CONST
	{
		$$ = A_NumConst(A_Pos(line, pos - len_int($1) + 1), $1);
	}
	|
	IDENTIFIER
	{
		$$ = A_IdExp(A_Pos(line, pos - len_id($1) + 1), $1);
	}
	|
	'(' EXP ')' 
	{
		$$ = A_EscExp(A_Pos($2->pos->line, $2->pos->pos - 1), NULL, $2);
	}
	|
	'(' '{' STM_LIST '}' EXP ')'
	{
		$$ = A_EscExp(A_Pos($3->head->pos->line, $3->head->pos->pos - 2), $3, $5);
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

int len_int(int x) {
	int ret = 0;
	while (x) {
		ret++;
		x /= 10;
	}
	return ret;
}

int len_id(char* s) {
	return strlen(s);
}