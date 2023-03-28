#include "type_check.h"
#include "symbol.h"
#include "types.h"

static bool error;
static S_table root;

static Ty_ty check_OpExp(FILE* out, S_table t, A_exp e);
static Ty_ty check_ArrayExp(FILE* out, S_table t, A_exp e);
static Ty_ty check_CallExp(FILE* out, S_table t, A_exp e);
static Ty_ty check_ClassVarExp(FILE* out, S_table t, A_exp e);
static Ty_ty check_BoolConst(FILE* out, S_table t, A_exp e);
static Ty_ty check_NumConst(FILE* out, S_table t, A_exp e);
static Ty_ty check_LengthExp(FILE* out, S_table t, A_exp e);
static Ty_ty check_IdExp(FILE* out, S_table t, A_exp e);
static Ty_ty check_ThisExp(FILE* out, S_table t, A_exp e);
static Ty_ty check_NewIntArrExp(FILE* out, S_table t, A_exp e);
static Ty_ty check_NewObjExp(FILE* out, S_table t, A_exp e);
static Ty_ty check_NotExp(FILE* out, S_table t, A_exp e);
static Ty_ty check_MinusExp(FILE* out, S_table t, A_exp e);
static Ty_ty check_EscExp(FILE* out, S_table t, A_exp e);
static Ty_ty check_Getint(FILE* out, S_table t, A_exp e);
static Ty_ty check_Getch(FILE* out, S_table t, A_exp e);
static Ty_ty check_Getarray(FILE* out, S_table t, A_exp e);
static Ty_ty check_Exp(FILE* out, S_table t, A_exp e);
static Ty_ty check_FuncExpList(FILE* out, S_table t, A_expList el);
static void check_ArrayExpList(FILE* out, S_table t, A_expList el);
static void check_VarDecl(FILE* out, S_table t, A_varDecl vd);
static void check_VarDeclList(FILE* out, S_table t, A_varDeclList vdl);
static void check_NestedStm(FILE* out, S_table t, A_stm s);
static void check_IfStm(FILE* out, S_table t, A_stm s);
static void check_WhileStm(FILE* out, S_table t, A_stm s);
static void check_AssignStm(FILE* out, S_table t, A_stm s);
static void check_ArrayInit(FILE* out, S_table t, A_stm s);
static void check_CallStm(FILE* out, S_table t, A_stm s);
static void check_Putint(FILE* out, S_table t, A_stm s);
static void check_Putch(FILE* out, S_table t, A_stm s);
static void check_Putarray(FILE* out, S_table t, A_stm s);
static void check_Stm(FILE* out, S_table t, A_stm s);
static void check_StmList(FILE* out, S_table t, A_stmList sl);
static void check_MainMethod(FILE* out, S_table t, A_mainMethod main);
static void check_ClassDeclList(FILE* out, S_table t, A_classDeclList cdl);

static Ty_ty check_OpExp(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty left = check_Exp(out, t, e->u.op.left);
    Ty_ty right = check_Exp(out, t, e->u.op.right);
    if (error || left == NULL || right == NULL) {
        return NULL;
    }
    if (left->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected in OpExp\n", e->u.op.left->pos->line, e->u.op.left->pos->pos);
        return NULL;
    }
    if (right->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected in OpExp\n", e->u.op.right->pos->line, e->u.op.right->pos->pos);
        return NULL;
    }
    return Ty_Int();
}

static Ty_ty check_ArrayExp(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty arr = check_Exp(out, t, e->u.array_pos.arr);
    Ty_ty arr_pos = check_Exp(out, t, e->u.array_pos.arr_pos);
    if (error || arr == NULL || arr_pos == NULL) {
        return NULL;
    }
    if (!arr->pointer) {
        error = TRUE;
        fprintf(out, "line %d:%d: an pointer type is expected\n", e->u.array_pos.arr->pos->line, e->u.array_pos.arr->pos->pos);        
        return NULL;        
    }
    if (arr->kind != Ty_array) {
        error = TRUE;
        fprintf(out, "line %d:%d: an array type is expected\n", e->u.array_pos.arr->pos->line, e->u.array_pos.arr->pos->pos);        
        return NULL;
    }
    if (arr_pos->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected as array's index\n", e->u.array_pos.arr->pos->line, e->u.array_pos.arr->pos->pos);        
        return NULL;
    }
    return Ty_LocationInt();
}

// TODO: next lab
static Ty_ty check_CallExp(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
}

// TODO: next lab
static Ty_ty check_ClassVarExp(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
}

static Ty_ty check_BoolConst(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    return Ty_Int();
}

static Ty_ty check_NumConst(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    return Ty_Int();
}

static Ty_ty check_LengthExp(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty ty = check_Exp(out, t, e->u.e);
    if (error || ty == NULL) {
        return NULL;
    }
    if (ty->kind != Ty_array) {
        error = TRUE;
        fprintf(out, "line %d:%d: an array type is expected as lengthExp's arguement\n", e->u.e->pos->line, e->u.e->pos->pos);        
        return NULL;
    }
    return Ty_Int();
}

static Ty_ty check_IdExp(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty ty = (Ty_ty)S_look(t, S_Symbol(e->u.v));
    if (ty == NULL) {
        error = TRUE;
        fprintf(out, "line: %d:%d: variable %s is not defined\n", e->pos->line, e->pos->pos, e->u.v);
        return NULL;
    }
    return ty;
}

// TODO: next lab
static Ty_ty check_ThisExp(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
}

static Ty_ty check_NewIntArrExp(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty ty = check_Exp(out, t, e->u.e);
    if (ty->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected as new array's size\n", e->u.e->pos->line, e->u.e->pos->pos);        
        return NULL;
    }
    return Ty_PointerArray();
}

// TODO: next lab
static Ty_ty check_NewObjExp(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
}

static Ty_ty check_NotExp(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty ty = check_Exp(out, t, e->u.e);
    if (error || ty == NULL) {
        return NULL;
    }
    if (ty->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected in NotExp\n", e->u.e->pos->line, e->u.e->pos->pos);        
        return NULL;  
    }
    return Ty_Int();
}

static Ty_ty check_MinusExp(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty ty = check_Exp(out, t, e->u.e);
    if (error || ty == NULL) {
        return NULL;
    }
    if (ty->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected in MinusExp\n", e->u.e->pos->line, e->u.e->pos->pos);        
        return NULL;  
    }
    return Ty_Int();
}

static Ty_ty check_EscExp(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    check_StmList(out, t, e->u.escExp.ns);
    if (error) {
        return NULL;
    }
    Ty_ty ty = check_Exp(out, t, e->u.escExp.exp);
    if (error || ty == NULL) {
        return NULL;
    }
    return ty;
}

static Ty_ty check_Getint(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    return Ty_Int();
}

static Ty_ty check_Getch(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    return Ty_Int();
}

static Ty_ty check_Getarray(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty ty = check_Exp(out, t, e->u.e);
    if (error || ty == NULL) {
        return NULL;
    }
    if (!ty->location) {
        error = TRUE;
        fprintf(out, "line %d:%d: a location type is expected in getarray()\n", e->u.e->pos->line, e->u.e->pos->pos);        
        return NULL;
    }
    if (ty->kind != Ty_array) {
        error = TRUE;
        fprintf(out, "line %d:%d: an array type is expected in getarray()\n", e->u.e->pos->line, e->u.e->pos->pos);        
        return NULL;
    }

    return NULL;
}

static Ty_ty check_Exp(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    switch (e->kind) {
    case A_opExp: return check_OpExp(out, t, e); break;
    case A_arrayExp: return check_ArrayExp(out, t, e); break;
    case A_callExp: return check_CallExp(out, t, e); break;
    case A_classVarExp: return check_ClassVarExp(out, t, e); break;
    case A_boolConst: return check_BoolConst(out, t, e); break;
    case A_numConst: return check_NumConst(out, t, e); break;
    case A_lengthExp: return check_LengthExp(out, t, e); break;
    case A_idExp: return check_IdExp(out, t, e); break;
    case A_thisExp: return check_ThisExp(out, t, e); break;
    case A_newIntArrExp: return check_NewIntArrExp(out, t, e); break;
    case A_newObjExp: return check_NewObjExp(out, t, e); break;
    case A_notExp: return check_NotExp(out, t, e); break;
    case A_minusExp: return check_MinusExp(out, t, e); break;
    case A_escExp: return check_EscExp(out, t, e); break;
    case A_getint: return check_Getint(out, t, e); break;
    case A_getch: return check_Getch(out, t, e); break;
    case A_getarray: return check_Getarray(out, t, e); break;
    default: error = TRUE; fprintf(out, "line %d:%d: Unknown expression kind!\n", e->pos->line, e->pos->pos); break;
    }
}

// TODO: next lab
static Ty_ty check_FuncExpList(FILE* out, S_table t, A_expList el) {
    return NULL;
}

static void check_ArrayExpList(FILE* out, S_table t, A_expList el) {
    if (!el || error) {
        return;
    }
    Ty_ty ty = check_Exp(out, t, el->head);
    if (error) {
        return;
    }
    if (ty != NULL && ty->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected in array initialization list\n", el->head->pos->line, el->head->pos->pos);        
        return;
    }
    if (el->tail) {
        check_ArrayExpList(out, t, el->tail);
    }
}

static void check_VarDecl(FILE* out, S_table t, A_varDecl vd) {
    if (!vd || error) {
        return;
    }
    if (S_look(t, S_Symbol(vd->v)) != NULL) {
        fprintf(out, "line %d:%d: '%s' variable redefinition\n", vd->pos->line, vd->pos->pos, vd->v);
        error = TRUE;
        return;
    }
    // declare
    switch (vd->t->t) {
    case A_intType: S_enter(t, S_Symbol(vd->v), Ty_LocationInt()); break;
    // TODO: next lab 
    // case A_idType: S_enter(t, S_Symbol(vd->v), Ty_Name()); break;
    case A_intArrType: S_enter(t, S_Symbol(vd->v), Ty_LocationPointerArray()); break;
    }
    
    // definition
    if (vd->elist) {
        if (vd->t->t == A_intType) {
            Ty_ty ty = check_Exp(out, t, vd->elist->head);
            if (error || ty == NULL) {
                return;
            }
            if(ty->kind != Ty_int) {
                error = TRUE;
                fprintf(out, "line: %d:%d: assign error: an int type is expected\n", vd->pos->line, vd->pos->pos);
            }
        } else if (vd->t->t == A_intArrType) {
            check_ArrayExpList(out, t, vd->elist);
            if (error) {
                return;
            }
        } else {
            fprintf(out, "line: %d:%d: Initialize non array?!\n");
        }
    }
}

static void check_VarDeclList(FILE* out, S_table t, A_varDeclList vdl) {
    if (!vdl || error) {
        return;
    }
    check_VarDecl(out, t, vdl->head);
    if (vdl->tail) {
        check_VarDeclList(out, t, vdl->tail);
    }
}

static void check_NestedStm(FILE* out, S_table t, A_stm s) {
    if (!s || error) {
        return;
    }
    check_StmList(out, t, s->u.ns);
}

static void check_IfStm(FILE* out, S_table t, A_stm s) {
    if (!s || error) {
        return;
    }
    Ty_ty ty = check_Exp(out, t, s->u.if_stat.e);
    if (error || ty == NULL) {
        return;
    }
    if (ty->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected in if()\n", s->u.if_stat.e->pos->line, s->u.if_stat.e->pos->pos);        
        return;
    }
    check_Stm(out, t, s->u.if_stat.s1);
    if (error) {
        return;
    }
    if (s->u.if_stat.s2) {
        check_Stm(out, t, s->u.if_stat.s2);
    }
}

static void check_WhileStm(FILE* out, S_table t, A_stm s) {
    if (!s || error) {
        return;
    }
    Ty_ty ty = check_Exp(out, t, s->u.while_stat.e);
    if (error || ty == NULL) {
        return;
    }
    if (ty->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected in while()\n", s->u.while_stat.e->pos->line, s->u.while_stat.e->pos->pos);
        return;
    }
    if (s->u.while_stat.e) {
        check_Stm(out, t, s->u.while_stat.s);
    }
}

// TODO: not completed
static void check_AssignStm(FILE* out, S_table t, A_stm s) {
    if (!s || error) {
        return;
    }
    Ty_ty arr = check_Exp(out, t, s->u.assign.arr);
    Ty_ty value = check_Exp(out, t, s->u.assign.value);
    if (error || arr == NULL || value == NULL) {
        return;
    }
    if (!arr->location) {
        error = TRUE;
        fprintf(out, "line %d:%d: a location type is expected in the left of AssignStm\n", s->u.assign.arr->pos->line, s->u.assign.arr->pos->pos);
        return;        
    }
    if (value->kind != arr->kind) {
        error = TRUE;
        if (arr->kind == Ty_int) {
            fprintf(out, "line %d:%d: an int type is expected in the right of AssignStm\n", s->u.assign.value->pos->line, s->u.assign.value->pos->pos);
        } else if (arr->kind == Ty_array) {
            fprintf(out, "line %d:%d: an array type is expected in the right of AssignStm\n", s->u.assign.value->pos->line, s->u.assign.value->pos->pos);
        }
        return;               
    }
}

static void check_ArrayInit(FILE* out, S_table t, A_stm s) {
    if (!s || error) {
        return;
    }
    Ty_ty arr = check_Exp(out, t, s->u.array_init.arr);
    if (error || arr == NULL) {
        return;
    }
    if (arr->kind != Ty_array) {
        error = TRUE;
        fprintf(out, "line %d:%d: an array type is expected in the left of AssignStm\n", s->u.array_init.arr->pos->line, s->u.array_init.arr->pos->pos);
        return;        
    }
    if (!arr->location) {
        error = TRUE;
        fprintf(out, "line %d:%d: an location type is expected in the left of AssignStm\n", s->u.array_init.arr->pos->line, s->u.array_init.arr->pos->pos);
        return; 
    }
    check_ArrayExpList(out, t, s->u.array_init.init_values);
}

// TODO: next lab
static void check_CallStm(FILE* out, S_table t, A_stm s) {
    return;
}

static void check_Putint(FILE* out, S_table t, A_stm s) {
    if (!s || error) {
        return;
    }
    Ty_ty ty = check_Exp(out, t, s->u.e);
    if (error || ty == NULL) {
        return;
    }
    if (ty->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected in putint()\n", s->u.e->pos->line, s->u.e->pos->pos);        
        return;
    }
}

static void check_Putch(FILE* out, S_table t, A_stm s) {
    if (!s || error) {
        return;
    }
    Ty_ty ty = check_Exp(out, t, s->u.e);
    if (error || ty == NULL) {
        return;
    }
    if (ty->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected in putch()\n", s->u.e->pos->line, s->u.e->pos->pos);        
        return;
    }
}

static void check_Putarray(FILE* out, S_table t, A_stm s) {
    if (!s || error) {
        return;
    }
    Ty_ty ty1 = check_Exp(out, t, s->u.putarray.e1);
    Ty_ty ty2 = check_Exp(out, t, s->u.putarray.e2);
    if (error || ty1 == NULL || ty2 == NULL) {
        return;
    }
    if (ty1->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected as the first arguement putarray()\n", s->u.putarray.e1->pos->line, s->u.putarray.e1->pos->pos);        
        return;
    }
    if (ty2->kind != Ty_array) {
        error = TRUE;
        fprintf(out, "line %d:%d: an array type is expected as the second arguement putarray()\n", s->u.putarray.e2->pos->line, s->u.putarray.e2->pos->pos);        
        return;
    }
}

static void check_Return(FILE* out, S_table t, A_stm s) {
    if (!s || error) {
        return;
    }
    Ty_ty ty = check_Exp(out, t, s->u.e);
    if (error || ty) {
        return;
    }
    if (ty->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected to return\n", s->u.e->pos->line, s->u.e->pos->pos);        
        return;
    }
}

static void check_Stm(FILE* out, S_table t, A_stm s) {
    if (!s || error) {
        return;
    }
    switch (s->kind) {
    case A_nestedStm: check_NestedStm(out, t, s); break;
    case A_ifStm: check_IfStm(out, t, s); break;
    case A_whileStm: check_WhileStm(out, t, s); break;
    case A_assignStm: check_AssignStm(out, t, s); break;
    case A_arrayInit: check_ArrayInit(out, t, s); break;
    case A_callStm: check_CallStm(out, t, s); break;
    case A_continue: break;
    case A_break: break;
    case A_return: check_Return(out, t, s); break;
    case A_putint: check_Putint(out, t, s); break;
    case A_putarray: check_Putarray(out, t, s); break;
    case A_putch: check_Putch(out, t, s); break;
    case A_starttime: break;
    case A_stoptime: break;
    default: error = TRUE; fprintf(out, "line %d:%d: Unknown statement kind!\n", s->pos->line, s->pos->pos); break;
    }
}

static void check_StmList(FILE* out, S_table t, A_stmList sl) {
    if (!sl || error) {
        return;
    }
    check_Stm(out, t, sl->head);
    if (sl->tail) {
        check_StmList(out, t, sl->tail);
    }
}

static void check_MainMethod(FILE* out, S_table t, A_mainMethod main) {
    if (main->vdl) {
        check_VarDeclList(out, t, main->vdl);
    }
    if (error) {
        return;
    }
    if (main->sl) {
        check_StmList(out, t, main->sl);
    }
}

// TODO: next lab
static void check_ClassDeclList(FILE* out, S_table t, A_classDeclList cdl) {
    if (!cdl || error) {
        return;
    }
    return;
}

void check_Prog(FILE* out, A_prog p) {
    root = S_empty();
    if (p->m) {
        S_table main = S_empty();
        S_enter(root, S_Symbol("main"), main);
        check_MainMethod(out, main, p->m);
    } else {
        fprintf(out, "Error: There's no main class!\n");
        error = TRUE;
    }
    if (error) {
        return;
    }
    if (p->cdl) {
        check_ClassDeclList(out, NULL, p->cdl);
    }
}