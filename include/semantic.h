#ifndef __SEMANTIC_H
#define __SEMANTIC_H

#include <stdio.h>
#include <string.h>
#include "symbol.h"
#include "types.h"
#include "temp.h"

// classEntry
typedef struct classEntry_ classEntry_;
typedef struct classEntry_ *classEntry; 
struct classEntry_ {
    S_table methodTable;
    S_table varTable;
    string id;
    int var_offset;
    int method_offset;
};
classEntry ClassEntry(string id);

// methodEntry
typedef struct methodEntry_ methodEntry_;
typedef struct methodEntry_ *methodEntry;
struct methodEntry_ {
    S_table localTable; // map var => ty 
    S_table tempTable; // map var => temp
    classEntry ce;
    classEntry from;
    Ty_ty ret;
    Ty_tyList fl;
    A_methodDecl md;
    int offset;
    Temp_temp this;
};
methodEntry MethodEntry(classEntry ce, classEntry from, Ty_ty ret, Ty_tyList fl, A_methodDecl md, int offset);

// varEntry
typedef struct varEntry_ varEntry_;
typedef struct varEntry_ *varEntry;
struct varEntry_ {
    Ty_ty ty;
    A_varDecl vd;
    int offset;
};
varEntry VarEntry(Ty_ty ty, A_varDecl vd, int offset);

// check function
Ty_ty check_OpExp(FILE* out, methodEntry me, A_exp e);
Ty_ty check_ArrayExp(FILE* out, methodEntry me, A_exp e);
Ty_ty check_CallExp(FILE* out, methodEntry me, A_exp e);
Ty_ty check_ClassVarExp(FILE* out, methodEntry me, A_exp e);
Ty_ty check_BoolConst(FILE* out, A_exp e);
Ty_ty check_NumConst(FILE* out, A_exp e);
Ty_ty check_LengthExp(FILE* out, methodEntry me, A_exp e);
Ty_ty check_IdExp(FILE* out, methodEntry me, A_exp e);
Ty_ty check_ThisExp(FILE* out, methodEntry me, A_exp e);
Ty_ty check_NewIntArrExp(FILE* out, methodEntry me, A_exp e);
Ty_ty check_NewObjExp(FILE* out, methodEntry me, A_exp e);
Ty_ty check_NotExp(FILE* out, methodEntry me, A_exp e);
Ty_ty check_MinusExp(FILE* out, methodEntry me, A_exp e);
Ty_ty check_EscExp(FILE* out, methodEntry me, A_exp e);
Ty_ty check_Getint(FILE* out, A_exp e);
Ty_ty check_Getch(FILE* out, A_exp e);
Ty_ty check_Getarray(FILE* out, methodEntry me, A_exp e);
Ty_ty check_Exp(FILE* out, methodEntry me, A_exp e);
void check_VarDecl(FILE* out, methodEntry me, S_table t, A_varDecl vd, int offset);
void check_VarDeclList(FILE* out, methodEntry me, classEntry ce, S_table t, A_varDeclList vdl, int offset);
void check_NestedStm(FILE* out, methodEntry me, A_stm s);
void check_IfStm(FILE* out, methodEntry me, A_stm s);
void check_WhileStm(FILE* out, methodEntry me, A_stm s);
void check_AssignStm(FILE* out, methodEntry me, A_stm s);
void check_ArrayInit(FILE* out, methodEntry me, A_stm s);
void check_CallStm(FILE* out, methodEntry me, A_stm s);
void check_Putint(FILE* out, methodEntry me, A_stm s);
void check_Putch(FILE* out, methodEntry me, A_stm s);
void check_Putarray(FILE* out, methodEntry me, A_stm s);
void check_Return(FILE* out, methodEntry me, A_stm s);
void check_Stm(FILE* out, methodEntry me, A_stm s);
void check_StmList(FILE* out, methodEntry me, A_stmList sl);
void check_MainMethod(FILE* out, methodEntry me, A_mainMethod main);
// class
void check_ClassDeclList(FILE* out, S_table t, A_classDeclList cdl);
void check_MethodDecl(FILE* out, S_table t, A_methodDecl md);
void check_MethodDeclList(FILE* out, S_table t, A_methodDeclList mdl);
void check_MethodDeclList_wrap(FILE* out, classEntry ce, A_methodDeclList mdl);
void parse_ClassDecl(FILE* out, classEntry ce, A_classDecl cd);
void parse_ClassDeclList(FILE* out, S_table t, A_classDeclList cdl);
void parse_ClassDeclListWithExtends(FILE* out, S_table t, A_classDeclList cdl);
void check_FormalList(FILE* out, S_table t, A_formalList fl);
void check_Prog(FILE* out, A_prog p);

#endif