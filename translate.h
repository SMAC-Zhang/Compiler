#ifndef __TRANSLATE_H
#define __TRANSLATE_H

#include "fdmjast.h"
#include "treep.h"
#include "symbol.h"
#include "semantic.h"

typedef struct Tr_exp_ *Tr_exp;
Tr_exp translate_Exp(FILE* out, methodEntry me, A_exp e);
Tr_exp translate_OpExp(FILE* out, methodEntry me, A_exp e);
Tr_exp translate_ArrayExp(FILE* out, methodEntry me, A_exp e);
Tr_exp translate_CallExp(FILE* out, methodEntry me, A_exp e);
Tr_exp translate_ClassVarExp(FILE* out, methodEntry me, A_exp e);
Tr_exp translate_BoolConst(FILE* out, methodEntry me, A_exp e);
Tr_exp translate_NumConst(FILE* out, methodEntry me, A_exp e);
Tr_exp translate_LengthExp(FILE* out, methodEntry me, A_exp e);
Tr_exp translate_IdExp(FILE* out, methodEntry me, A_exp e);
Tr_exp translate_ThisExp(FILE* out, methodEntry me, A_exp e);
Tr_exp translate_NewIntArrExp(FILE* out, methodEntry me, A_exp e);
Tr_exp translate_NewObjExp(FILE* out, methodEntry me, A_exp e);
Tr_exp translate_NotExp(FILE* out, methodEntry me, A_exp e);
Tr_exp translate_MinusExp(FILE* out, methodEntry me, A_exp e);
Tr_exp translate_EscExp(FILE* out, methodEntry me, A_exp e);
Tr_exp translate_Getint(FILE* out, methodEntry me, A_exp e);
Tr_exp translate_Getch(FILE* out, methodEntry me, A_exp e);

Temp_temp translate_Formal(FILE* out, methodEntry me, A_formal f);
Temp_tempList translate_FormalList(FILE* out, methodEntry me, A_formalList fl);

T_funcDecl tranlate_MethodDecl(FILE* out, classEntry ce, A_methodDecl md);
T_funcDeclList translate_MethodDeclList(FILE* out, classEntry ce, A_methodDeclList mdl);
T_funcDeclList translate_ClassDecl(FILE* out, A_classDecl cd);
T_funcDeclList translate_ClassDeclList(FILE* out, A_classDeclList cdl);
T_funcDecl translate_MainMethod(FILE* out, methodEntry me, A_mainMethod main);
T_funcDeclList translate_Prog(FILE* out, A_prog p);

Tr_exp translate_VarDeclList(FILE* out, methodEntry me, A_varDeclList vdl);
Tr_exp translate_VarDecl(FILE* out, methodEntry me, A_varDecl vd);

Tr_exp translate_StmList(FILE* out, methodEntry me, A_stmList sl);
Tr_exp translate_Stm(FILE* out, methodEntry me, A_stm s);


Tr_exp translate_NestedStm(FILE* out, methodEntry me, A_stm s);
Tr_exp translate_IfStm(FILE* out, methodEntry me, A_stm s);
Tr_exp translate_WhileStm(FILE* out, methodEntry me, A_stm s);
Tr_exp translate_AssignStm(FILE* out, methodEntry me, A_stm s);
Tr_exp translate_ArrayInit(FILE* out, methodEntry me, A_stm s);
Tr_exp translate_CallStm(FILE* out, methodEntry me, A_stm s);
Tr_exp translate_Continue(FILE* out, methodEntry me, A_stm s);
Tr_exp translate_Break(FILE* out, methodEntry me, A_stm s);
Tr_exp translate_Return(FILE* out, methodEntry me, A_stm s);
Tr_exp translate_Putint(FILE* out, methodEntry me, A_stm s);
Tr_exp translate_Putch(FILE* out, methodEntry me, A_stm s);
Tr_exp translate_Putarray(FILE* out, methodEntry me, A_stm s);
Tr_exp translate_Starttime(FILE* out, methodEntry me, A_stm s);
Tr_exp translate_Stoptime(FILE* out, methodEntry me, A_stm s);

#endif