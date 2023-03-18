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

// extern int yydebug = 1;

%}

%union
{
    A_pos token;
    string id;
    A_prog prog;
    A_type type;
    A_mainMethod main;
    A_stm stm;
	A_stmList stmlist;
	A_varDecl var;
	A_varDeclList varlist;
	A_classDecl classdec;
	A_classDeclList classdeclist;
	A_methodDecl method;
	A_methodDeclList methodlist;
	A_formalList formallist;
    A_exp exp;
	A_expList explist;
	A_type type;
}

// 运算符
%token <token> OP_PLUS OP_MULTIPLY OP_MINUS OP_DIV UMINUS
%token <token> OR AND LESS LE EQ NOT GREATER GE
//两种左括号，单独处理以获得pos
%token <token> BRACE PARENTHESIS
// 关键字
%token <token> PUBLIC INT MAIN PUTCH PUTINT
%token <token> CLASS EXTENDS
%token <token> IF ELSE WHILE CONTINUE BREAK RETURN
%token <token> PUTINT PUTCH PUTARRAY
%token <token> GETINT GETCH GETARRAY
%token <token> STARTTIME STOPTIME
%token <token> TRUE FALSE LENGTH
%token <token> THIS NEW
// 数字
%token <exp> INT_CONST
// 标识符
%token <id> IDENTIFIER

// 非终结符的类
%type <prog> PROG
%type <main> MAIN_METHOD
%type <stm> STM
%type <stmlist> STM_LIST
%type <var> VAR
%type <varlist> VAR_LIST
%type <classdec> CLASS_DEC
%type <classdeclist> CLASS_DEC_LIST
%type <method> METHOD
%type <methodlist> METHOD_LIST
%type <formallist> FORMAL_LIST FORMAL_REST
%type <exp> EXP
%type <explist> EXP_LIST EXP_REST
%type <type> TYPE
%type <explist> INT_CONST_LIST INT_CONST_REST

%start PROG
%left OR
%left AND
%left OP_PLUS OP_MINUS
%left OP_MULTIPLY OP_DIV
%left UMINUS NOT

%%
PROG: MAIN_METHOD CLASS_LIST
	{
		root = A_Prog($1->pos, $1, $2);
		$$ = A_Prog($1->pos, $1, $2);
	}

MAIN_METHOD: PUBLIC INT MAIN PARENTHESIS ')' BRACE VAR_LIST STM_LIST '}'
	{
		$$ = A_MainMethod($1, $7, $8);
	}

CLASS_DEC:
	CLASS IDENTIFIER BRACE VAR_LIST METHOD_LIST '}' 
	{
		$$ = A_ClassDecl($1, $2, NULL, $4, $5);
	}
	|
	CLASS IDENTIFIER EXTENDS IDENTIFIER BRACE VAR_LIST METHOD_LIST '}' 
	{
		$$ = A_ClassDecl($1, $2, $4, $6, $7);
	}

CLASS_DEC_LIST:
	CLASS_DEC CLASS_DEC_LIST
	{
		$$ = A_ClassDeclList($1, $2);
	}
	|
	{
		$$ = A_ClassDeclList(NULL, NULL);
	}

VAR:
	TYPE IDENTIFIER ';'
	{
		$$ = A_VarDecl($1->pos, $1, $2, NULL);
	}
	|
	TYPE IDENTIFIER '=' INT_CONST ';'
	{
		$$ = A_VarDecl($1->pos, $1, $2, A_ExpList($4, NULL));
	}
	|
	TYPE IDENTIFIER '=' BRACE INT_CONST_LIST '}' ';'
	{
		$$ = A_VarDecl($1->pos, $1, $2, $5);
	}

VAR_LIST:
	VAR VAR_LIST
	{
		$$ = A_VarDeclList($1, $2);
	}
	|
	{
		$$ = A_VarDeclList(NULL, NULL);
	}

INT_CONST_LIST:
	INT_CONST INT_CONST_REST
	{
		$$ = A_ExpList($1, $2);
	}
	|
	{
		$$ = A_ExpList(NULL, NULL);
	}

INT_CONST_REST:
	',' INT_CONST INT_CONST_REST
	{
		$$ = A_ExpList($2, $3);
	}
	|
	{
		$$ = A_ExpList(NULL, NULL);
	}

METHOD:
	PUBLIC TYPE IDENTIFIER PARENTHESIS FORMAL_LIST ')' BRACE VAR_LIST STM_LIST '}'
	{
		$$ = A_MethodDecl($1, $2, $3, $5, $8, $9);
	}

METHOD_LIST:
	METHOD METHOD_LIST
	{
		$$ = A_MethodDeclList($1, $2);
	}
	|
	{
		$$ = A_MethodDeclList(NULL, NULL);
	}

FORMAL_LIST:
	TYPE IDENTIFIER FORMAL_REST
	{
		$$ = A_FormalList(A_Formal($1->pos, $1, $2), $3);
	}
	|
	{
		$$ = A_FormalList(NULL, NULL);
	}

FORMAL_REST:
	',' TYPE IDENTIFIER FORMAL_REST
	{
		$$ = A_FormalList(A_Formal($2->pos, $2, $3), $4);
	}
	|
	{
		$$ = A_FormalList(NULL, NULL);
	}

TYPE:
	CLASS IDENTIFIER
	{
		$$ = A_Type($1, A_idType, $2);
	}
	|
	INT
	{
		$$ = A_Type($1, A_intType, NULL);
	}
	|
	INT '[' ']'
	{
		$$ = A_Type($1, A_intArrType, NULL);
	}

STM:
	BRACE STM_LIST '}'
	{
		$$ = A_NestedStm($1, $2);
	}
	|
	IF PARENTHESIS EXP ')' STM ELSE STM
	{
		$$ = A_IfStm($1, $3, $5, $7);
	}
	|
	IF PARENTHESIS EXP ')' STM
	{
		$$ = A_IfStm($1, $3, $5, NULL);
	}
	|
	WHILE PARENTHESIS EXP ')' STM
	{
		$$ = A_WhileStm($1, $3, $5);
	}
	|
	WHILE PARENTHESIS EXP ')' ';'
	{
		$$ = A_WhileStm($1, $3, NULL);
	}
	|
	EXP '=' EXP ';'
	{
		$$ = A_AssignStm($1->pos, $1, NULL, $3);
	}
	|
	EXP '[' EXP ']' '=' EXP ';'
	{
		$$ = A_AssignStm($1->pos, $1, $3, $6);
	}
	|
	EXP '[' ']' '=' BRACE EXP_LIST '}' ';'
	{
		$$ = A_ArrayInit($1->pos, $1, $6);
	}
	|
	EXP '.' IDENTIFIER PARENTHESIS EXP_LIST ')' ';'
	{
		$$ = A_CallStm($1->pos, $1, $3, $5);
	}
	|
	CONTINUE ';'
	{
		$$ = A_Continue($1);
	}
	|
	BREAK ';'
	{
		$$ = A_Break($1);
	}
	|
	RETURN ';'
	{
		$$ = A_Return($1);
	}
	|
	PUTINT PARENTHESIS EXP ')' ';'
	{
		$$ = A_Putint($1, $3);
	}
	|
	PUTCH PARENTHESIS EXP ')' ';'
	{
		$$ = A_Putch($1, $3);
	}
	|
	PUTARRAT PARENTHESIS EXP ',' EXP ')' ';'
	{
		$$ = A_Putarray($1, $3, $5);
	}
	|
	STARTTIME PARENTHESIS ')' ';'
	{
		$$ = A_Starttime($1);
	}
	|
	STOPTIME PARENTHESIS ')' ';'
	{
		$$ = A_Stoptime($1);
	}

STM_LIST:
	STM STM_LIST
	{
		$$ = A_StmList($1, $2);
	}
	|
	{
		$$ = A_StmList(NULL, NULL);
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
	EXP AND EXP
	{
		$$ = A_OpExp($1->pos, $1, A_and, $3);
	}
	|
	EXP OR EXP
	{
		$$ = A_OpExp($1->pos, $1, A_or, $3);
	}
	|
	EXP LESS EXP
	{
		$$ = A_OpExp($1->pos, $1, A_less, $3);
	}
	|
	EXP LE EXP
	{
		$$ = A_OpExp($1->pos, $1, A_le, $3);
	}
	|
	EXP EQ EXP
	{
		$$ = A_OpExp($1->pos, $1, A_eq, $3);
	}
	|
	EXP '[' EXP ']'
	{
		$$ = A_ArrayExp($1->pos, $1, $3);
	}
	|
	EXP '.' IDENTIFIER PARENTHESIS EXP_LIST ')'
	{
		$$ = A_CallExp($1->pos, $1, $3, $5);
	}
	|
	EXP '.' IDENTIFIER
	{
		$$ = A_ClassVarExp($1->pos, $1, $3, NULL);
	}
	|
	EXP '.' IDENTIFIER '[' EXP ']'
	{
		$$ = A_ClassVarExp($1->pos, $1, $3, $5);
	}
	|
	INT_CONST
	{
		$$ = $1;
	}
	|
	TRUE
	{
		$$ = A_BoolConst($1, 1);
	}
	|
	FALSE
	{
		$$ = A_BoolConst($1, 0);
	}
	|
	LENGTH PARENTHESIS EXP ')'
	{
		$$ = A_LengthExp($1, $3);
	}
	|
	IDENTIFIER
	{
		$$ = A_IdExp(A_Pos(line, pos - strlen($1)), $1);
	}
	|
	THIS
	{
		$$ = A_ThisExp($1);
	}
	|
	NEW INT '[' EXP ']'
	{
		$$ = A_NewIntArrExp($1, $4);
	}
	|
	NEW IDENTIFIER PARENTHESIS ')'
	{
		$$ = A_NewObjExp($1, $2);
	}
	|
	NOT EXP %prec NOT
	{
		$$ = A_NotExp($1, $2);
	}
	|
	OP_MINUS EXP %prec UMINUS
	{
		$$ = A_MinusExp($1, $2);
	}
	|
	PARENTHESIS EXP ')' 
	{
		$$ = A_EscExp($1, NULL, $2);
	}
	|
	PARENTHESIS '{' STM_LIST '}' EXP ')'
	{
		$$ = A_EscExp($1, $3, $5);
	}
	|
	GETINT '(' ')'
	{
		$$ = A_Getint($1);
	}
	|
	GETCH '(' ')'
	{
		$$ = A_Getch($1);
	}
	|
	GETARRAY '(' EXP ')'
	{
		$$ = A_Getarray($1, $3);
	}

EXP_LIST:
	EXP EXP_REST
	{
		$$ = A_ExpList($1, $2);
	}
	|
	{
		$$ = A_ExpList(NULL, NULL);
	}

EXP_REST
	',' EXP EXP_REST
	{
		$$ = A_ExpList($2, $3);
	}
	|
	{
		$$ = A_ExpList(NULL, NULL);
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