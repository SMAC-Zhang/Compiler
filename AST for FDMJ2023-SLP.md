# AST for FDMJ2023

```
//This is an AST definition for FDMJ2023-SLP
//In your assignment, you may use the AST for the FDMJ2023 full grammar
//The AST below may be used, but mostly for the purpose of 
//using a simplified AST to explain AST for a "real" program language

#ifndef __ASTSLP
#define __ASTSLP
#include "util.h"

typedef struct A_pos_* A_pos; //position information
typedef struct A_prog_* A_prog;
typedef struct A_mainClass_* A_mainClass;
typedef struct A_stmList_* A_stmList;
typedef struct A_stm_* A_stm;
typedef struct A_exp_* A_exp;

struct A_pos_ {
    int line, pos;
};

struct A_prog_ {
    A_pos pos;
    A_mainClass m;
};

struct A_mainClass_ {
    A_pos pos;
    A_stmList sl;
};

struct A_stmList_ {
    A_stm head;
    A_stmList tail;
};

typedef enum {
    A_assignStm,
    A_putint,
    A_putch
} A_stmKind;

struct A_stm_ {
    A_pos pos;
    A_stmKind kind;
    union {
        struct {
            A_exp arr; //left must be a location for an array type
            A_exp pos; //array position
            A_exp value;
        } assign;
        A_exp e;
    } u;
};

typedef enum {
    A_plus,
    A_minus,
    A_times,
    A_div
} A_binop;

typedef enum {
    A_opExp,
    A_numConst,
    A_idExp,
    A_minusExp,
    A_escExp
} A_expKind;
    
struct A_exp_ {
    A_pos pos;
    A_expKind kind;
    union {
        struct {
                A_exp left;
                A_binop oper;
                A_exp right;
        } op;
        struct {
                A_stmList ns;
                A_exp exp;
        } escExp;
        int num;
        string v;
        A_exp e;
    } u;
};

/* All we need to build AST for FDMJ2023-SLP are the following
functions (from the AST for the full FDMJ2023 grammar), with NULL
in place for compatibility for the AST for FDMJ2023 (full grammar) 

A_pos A_Pos(int,int);
A_prog A_Prog(A_pos, A_mainClass, NULL);
A_mainClass A_MainClass(A_pos, NULL, A_stmList);

A_stmList A_StmList(A_stm, A_stmList);
A_stm A_Putint(A_pos, A_exp);
A_stm A_Putch(A_pos, A_exp);

A_exp A_OpExp(A_pos, A_exp, A_binop, A_exp);
A_exp A_NumConst(A_pos, int);
A_exp A_IdExp(A_pos, string);
A_exp A_MinusExp(A_pos, A_exp);
A_exp A_EscExp(A_pos, A_stmList, A_exp);

*/

#endif
```
