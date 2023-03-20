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
%token <token> OR AND LESS LE EQ NOT GREATER GE NEQ
//三种左括号，单独处理以获得pos
%token <token> BRACE BRACKET PARENTHESIS
// 关键字
%token <token> PUBLIC INT MAIN
%token <token> CLASS EXTENDS
%token <token> IF ELSE WHILE CONTINUE BREAK RETURN
%token <token> PUTINT PUTCH PUTARRAY
%token <token> GETINT GETCH GETARRAY
%token <token> STARTTIME STOPTIME
%token <token> True False LENGTH
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
%type <exp> EXP IntConst
%type <explist> EXP_LIST EXP_REST	
%type <type> TYPE
%type <explist> INT_CONST_LIST INT_CONST_REST

%start PROG
%left ']' ')'
%left '='
%left OR
%left AND
%left EQ NEQ
%left LESS LE GREATER GE
%left OP_PLUS OP_MINUS
%left OP_MULTIPLY OP_DIV
%left UMINUS
%right NOT
%left IDENTIFIER
%left PARENTHESIS BRACKET '.'
%left IF
%left ELSE

%%
PROG: MAIN_METHOD CLASS_DEC_LIST
	{
		root = A_Prog($1->pos, $1, $2);
		$$ = A_Prog($1->pos, $1, $2);
	}

MAIN_METHOD: PUBLIC INT MAIN PARENTHESIS ')' BRACE VAR_LIST STM_LIST '}'
	{
		$$ = A_MainMethod($1, $7, $8);
	}

CLASS_DEC:
	PUBLIC CLASS IDENTIFIER BRACE VAR_LIST METHOD_LIST '}' 
	{
		$$ = A_ClassDecl($1, $3, NULL, $5, $6);
	}
	|
	PUBLIC CLASS IDENTIFIER EXTENDS IDENTIFIER BRACE VAR_LIST METHOD_LIST '}' 
	{
		$$ = A_ClassDecl($1, $3, $5, $7, $8);
	}

CLASS_DEC_LIST:
	CLASS_DEC CLASS_DEC_LIST
	{
		$$ = A_ClassDeclList($1, $2);
	}
	|
	{
		$$ = NULL;
	}

VAR:
	TYPE IDENTIFIER ';'
	{
		$$ = A_VarDecl($1->pos, $1, $2, NULL);
	}
	|
	TYPE IDENTIFIER '=' IntConst ';'
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
		$$ = NULL;
	}

IntConst:
	INT_CONST
	{
		$$ = $1;
	} 
	|
	OP_MINUS INT_CONST %prec UMINUS
	{
		$$ = A_NumConst(A_Pos($2->pos->line, $2->pos->pos - 1), -$2->u.num);
	}

INT_CONST_LIST:
	IntConst INT_CONST_REST
	{
		$$ = A_ExpList($1, $2);
	}
	|
	{
		$$ = NULL;
	}

INT_CONST_REST:
	',' IntConst INT_CONST_REST
	{
		$$ = A_ExpList($2, $3);
	}
	|
	{
		$$ = NULL;
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
		$$ = NULL;
	}

FORMAL_LIST:
	TYPE IDENTIFIER FORMAL_REST
	{
		$$ = A_FormalList(A_Formal($1->pos, $1, $2), $3);
	}
	|
	{
		$$ = NULL;
	}

FORMAL_REST:
	',' TYPE IDENTIFIER FORMAL_REST
	{
		$$ = A_FormalList(A_Formal($2->pos, $2, $3), $4);
	}
	|
	{
		$$ = NULL;
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
	INT BRACKET ']'
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
	IF PARENTHESIS EXP ')' STM %prec IF
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
	EXP BRACKET EXP ']' '=' EXP ';'
	{
		$$ = A_AssignStm($1->pos, $1, $3, $6);
	}
	|
	EXP BRACKET ']' '=' BRACE EXP_LIST '}' ';'
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
	RETURN EXP ';'
	{
		$$ = A_Return($1, $2);
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
	PUTARRAY PARENTHESIS EXP ',' EXP ')' ';'
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
		$$ = NULL;
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
	EXP GREATER EXP
	{
		$$ = A_OpExp($1->pos, $1, A_greater, $3);
	}
	|
	EXP GE EXP
	{
		$$ = A_OpExp($1->pos, $1, A_ge, $3);
	}
	|
	EXP EQ EXP
	{
		$$ = A_OpExp($1->pos, $1, A_eq, $3);
	}
	|
	EXP NEQ EXP
	{
		$$ = A_OpExp($1->pos, $1, A_ne, $3);
	}
	|
	EXP BRACKET EXP ']'
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
	EXP '.' IDENTIFIER BRACKET EXP ']'
	{
		$$ = A_ClassVarExp($1->pos, $1, $3, $5);
	}
	|
	INT_CONST
	{
		$$ = $1;
	}
	|
	True
	{
		$$ = A_BoolConst($1, TRUE);
	}
	|
	False
	{
		$$ = A_BoolConst($1, FALSE);
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
	NEW INT BRACKET EXP ']'
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
	PARENTHESIS BRACE STM_LIST '}' EXP ')'
	{
		$$ = A_EscExp($1, $3, $5);
	}
	|
	GETINT PARENTHESIS ')'
	{
		$$ = A_Getint($1);
	}
	|
	GETCH PARENTHESIS ')'
	{
		$$ = A_Getch($1);
	}
	|
	GETARRAY PARENTHESIS EXP ')'
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
		$$ = NULL;
	}

EXP_REST:
	',' EXP EXP_REST
	{
		$$ = A_ExpList($2, $3);
	}
	|
	{
		$$ = NULL;
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