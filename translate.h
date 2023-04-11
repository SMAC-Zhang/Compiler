#ifndef __TRANSLATE_H
#define __TRANSLATE_H

#include "fdmjast.h"
#include "treep.h"

T_exp translate_Exp(FILE* out, A_exp e, Temp_label starts, Temp_label ends, bool loop);
T_exp translate_OpExp(FILE* out, A_exp e, Temp_label starts, Temp_label ends, bool loop);
T_exp translate_BoolConst(FILE* out, A_exp e);
T_exp translate_NumConst(FILE* out, A_exp e);
T_exp translate_LengthExp(FILE* out, A_exp e);
T_exp translate_IdExp(FILE* out, A_exp e);
T_exp translate_NotExp(FILE* out, A_exp e, Temp_label starts, Temp_label ends, bool loop);
T_exp translate_MinusExp(FILE* out, A_exp e, Temp_label starts, Temp_label ends, bool loop);
T_exp translate_EscExp(FILE* out, A_exp e, Temp_label starts, Temp_label ends, bool loop);
T_exp translate_Getint(FILE* out, A_exp e);
T_exp translate_Getch(FILE* out, A_exp e);

T_funcDecl translate_MainMethod(FILE* out, A_mainMethod main);
T_funcDeclList translate_Prog(FILE* out, A_prog p);

T_stm translate_VarDeclList(FILE* out, A_varDeclList vdl);
T_stm translate_VarDecl(FILE* out, A_varDecl vd);

T_stm translate_StmList(FILE* out, A_stmList sl, Temp_label starts, Temp_label ends, bool loop);
T_stm translate_Stm(FILE* out, A_stm s, Temp_label starts, Temp_label ends, bool loop);

T_stm translate_NestedStm(FILE* out, A_stm s, Temp_label starts, Temp_label ends, bool loop);
T_stm translate_IfStm(FILE* out, A_stm s, Temp_label starts, Temp_label ends, bool loop);
T_stm translate_WhileStm(FILE* out, A_stm s, Temp_label starts, Temp_label ends, bool loop);
T_stm translate_AssignStm(FILE* out, A_stm s, Temp_label starts, Temp_label ends, bool loop);
T_stm translate_Continue(FILE* out, A_stm s, Temp_label starts, bool loop);
T_stm translate_Break(FILE* out, A_stm s, Temp_label ends, bool loop);
T_stm translate_Return(FILE* out, A_stm s, Temp_label starts, Temp_label ends, bool loop);
T_stm translate_Putint(FILE* out, A_stm s, Temp_label starts, Temp_label ends, bool loop);
T_stm translate_Putch(FILE* out, A_stm s, Temp_label starts, Temp_label ends, bool loop);
T_stm translate_Starttime(FILE* out, A_stm s);
T_stm translate_Stoptime(FILE* out, A_stm s);

#endif