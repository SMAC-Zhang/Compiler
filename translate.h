#ifndef __TRANSLATE_H
#define __TRANSLATE_H

#include "fdmjast.h"
#include "treep.h"

typedef struct Tr_exp_ *Tr_exp;
Tr_exp translate_Exp(FILE* out, A_exp e);
Tr_exp translate_OpExp(FILE* out, A_exp e);
Tr_exp translate_BoolConst(FILE* out, A_exp e);
Tr_exp translate_NumConst(FILE* out, A_exp e);
Tr_exp translate_LengthExp(FILE* out, A_exp e);
Tr_exp translate_IdExp(FILE* out, A_exp e);
Tr_exp translate_NotExp(FILE* out, A_exp e);
Tr_exp translate_MinusExp(FILE* out, A_exp e);
Tr_exp translate_EscExp(FILE* out, A_exp e);
Tr_exp translate_Getint(FILE* out, A_exp e);
Tr_exp translate_Getch(FILE* out, A_exp e);

T_funcDecl translate_MainMethod(FILE* out, A_mainMethod main);
T_funcDeclList translate_Prog(FILE* out, A_prog p);

Tr_exp translate_VarDeclList(FILE* out, A_varDeclList vdl);
Tr_exp translate_VarDecl(FILE* out, A_varDecl vd);

Tr_exp translate_StmList(FILE* out, A_stmList sl);
Tr_exp translate_Stm(FILE* out, A_stm s);

Tr_exp translate_NestedStm(FILE* out, A_stm s);
Tr_exp translate_IfStm(FILE* out, A_stm s);
Tr_exp translate_WhileStm(FILE* out, A_stm s);
Tr_exp translate_AssignStm(FILE* out, A_stm s);
Tr_exp translate_Continue(FILE* out, A_stm s);
Tr_exp translate_Break(FILE* out, A_stm s);
Tr_exp translate_Return(FILE* out, A_stm s);
Tr_exp translate_Putint(FILE* out, A_stm s);
Tr_exp translate_Putch(FILE* out, A_stm s);
Tr_exp translate_Starttime(FILE* out, A_stm s);
Tr_exp translate_Stoptime(FILE* out, A_stm s);

#endif