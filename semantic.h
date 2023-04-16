#ifndef __TYPE_CHECK_H
#define __TYPE_CHECK_H

#include <stdio.h>
#include <string.h>

#include "semantic.h"
#include "symbol.h"
#include "types.h"

Ty_ty check_OpExp(FILE* out, S_table t, A_exp e);
Ty_ty check_ArrayExp(FILE* out, S_table t, A_exp e);
Ty_ty check_CallExp(FILE* out, S_table t, A_exp e);
Ty_ty check_ClassVarExp(FILE* out, S_table t, A_exp e);
Ty_ty check_BoolConst(FILE* out, S_table t, A_exp e);
Ty_ty check_NumConst(FILE* out, S_table t, A_exp e);
Ty_ty check_LengthExp(FILE* out, S_table t, A_exp e);
Ty_ty check_IdExp(FILE* out, S_table t, A_exp e);
Ty_ty check_ThisExp(FILE* out, S_table t, A_exp e);
Ty_ty check_NewIntArrExp(FILE* out, S_table t, A_exp e);
Ty_ty check_NewObjExp(FILE* out, S_table t, A_exp e);
Ty_ty check_NotExp(FILE* out, S_table t, A_exp e);
Ty_ty check_MinusExp(FILE* out, S_table t, A_exp e);
Ty_ty check_EscExp(FILE* out, S_table t, A_exp e);
Ty_ty check_Getint(FILE* out, S_table t, A_exp e);
Ty_ty check_Getch(FILE* out, S_table t, A_exp e);
Ty_ty check_Getarray(FILE* out, S_table t, A_exp e);
Ty_ty check_Exp(FILE* out, S_table t, A_exp e);
bool check_FuncExpList(FILE* out, S_table t, A_expList el, Ty_tyList fl);
void check_ArrayExpList(FILE* out, S_table t, A_expList el);
void check_VarDecl(FILE* out, S_table t, A_varDecl vd);
void check_VarDeclList(FILE* out, S_table t, A_varDeclList vdl);
void check_NestedStm(FILE* out, S_table t, A_stm s);
void check_IfStm(FILE* out, S_table t, A_stm s);
void check_WhileStm(FILE* out, S_table t, A_stm s);
void check_AssignStm(FILE* out, S_table t, A_stm s);
void check_ArrayInit(FILE* out, S_table t, A_stm s);
void check_CallStm(FILE* out, S_table t, A_stm s);
void check_Putint(FILE* out, S_table t, A_stm s);
void check_Putch(FILE* out, S_table t, A_stm s);
void check_Putarray(FILE* out, S_table t, A_stm s);
void check_Stm(FILE* out, S_table t, A_stm s);
void check_StmList(FILE* out, S_table t, A_stmList sl);
void check_MainMethod(FILE* out, S_table t, A_mainMethod main);
// class
void check_ClassDeclList(FILE* out, S_table t, A_classDeclList cdl);
void check_MethodDecl(FILE* out, S_table t, A_methodDecl md);
void check_MethodDeclList(FILE* out, S_table t, A_methodDeclList mdl);
void check_MethodDeclList_wrap(FILE* out, S_table t, A_methodDeclList mdl);
void parse_ClassDecl(FILE* out, S_table t, A_classDecl cd);
void parse_ClassDeclList(FILE* out, S_table t, A_classDeclList cdl);
void parse_ClassDeclListWithExtends(FILE* out, S_table t, A_classDeclList cdl);
Ty_tyList Ty_TyList(FILE* out, A_formalList fl);
bool check_Class(Ty_ty fa, Ty_ty son);
void check_FormalList(FILE* out, S_table t, A_formalList fl);
void check_Prog(FILE* out, A_prog p);

#endif