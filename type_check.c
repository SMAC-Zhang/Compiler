#include <string.h>
#include "type_check.h"
#include "symbol.h"
#include "types.h"

static bool error;
static S_table mainTable;
static S_table classTable;
static S_table inheritTable;
static string method_str = "method&";
static string local_str = "local&";
static string var_str = "var&";

typedef struct classNode_ {
    A_classDecl base;
    A_classDecl fa;
    int vis;
} classNode_;
typedef classNode_* classNode;

classNode ClassNode(A_classDecl base, A_classDecl fa, int vis) {
    classNode node = checked_malloc(sizeof(classNode));
    node->base = base;
    node->fa = fa;
    node->vis = vis;
    return node;
}

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
static bool check_FuncExpList(FILE* out, S_table t, A_expList el, Ty_tyList fl);
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
// class
static void check_ClassDeclList(FILE* out, S_table t, A_classDeclList cdl);
static void check_MethodDecl(FILE* out, S_table t, A_methodDecl md);
static void check_MethodDeclList(FILE* out, S_table t, A_methodDeclList mdl);
static void check_MethodDeclList_wrap(FILE* out, S_table t, A_methodDeclList mdl);
static void parse_ClassDecl(FILE* out, S_table t, A_classDecl cd);
static void parse_ClassDeclList(FILE* out, S_table t, A_classDeclList cdl);
static void parse_ClassDeclListWithExtends(FILE* out, S_table t, A_classDeclList cdl);
static Ty_tyList Ty_TyList(FILE* out, A_formalList fl);
static bool check_Class(Ty_ty fa, Ty_ty son);
static void check_FormalList(FILE* out, S_table t, A_formalList fl);


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
    if (arr->kind != Ty_array) {
        error = TRUE;
        fprintf(out, "line %d:%d: an array type is expected\n", e->u.array_pos.arr->pos->line, e->u.array_pos.arr->pos->pos);        
        return NULL;
    }
    if (!arr->pointer) {
        error = TRUE;
        fprintf(out, "line %d:%d: an pointer type is expected\n", e->u.array_pos.arr->pos->line, e->u.array_pos.arr->pos->pos);        
        return NULL;        
    }
    if (arr_pos->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected as array's index\n", e->u.array_pos.arr->pos->line, e->u.array_pos.arr->pos->pos);        
        return NULL;
    }
    return Ty_LocationInt();
}

static Ty_ty check_CallExp(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty obj = check_Exp(out, t, e->u.call.obj);
    if (obj->kind != Ty_class) {
        error = TRUE;
        fprintf(out, "line %d:%d: '%s' is not a class type\n", e->pos->line, e->pos->pos, e->u.call.obj->u.v);
        return NULL;
    }
    S_table class = S_look(classTable, S_Symbol(obj->id));
    S_table method = S_look(class, S_Symbol(method_str));
    Ty_ty fun = S_look(method, S_Symbol(e->u.call.fun));
    if (fun == NULL) {
        error = TRUE;
        fprintf(out, "line %d:%d: class '%s' doesn't have function %s\n", e->pos->line, e->pos->pos, obj->id, e->u.call.fun);
        return NULL;
    }
    if (check_FuncExpList(out, t, e->u.call.el, fun->fl) == FALSE) {
        if (error) {
            return NULL;
        }
        error = TRUE;
        fprintf(out, "line %d:%d: argument type error", e->pos->line, e->pos->pos);
        return NULL;
    }
    return fun->ret;
}

static Ty_ty check_ClassVarExp(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty obj = check_Exp(out, t, e->u.classvar.obj);
    if (error || obj == NULL) {
        return NULL;
    }
    if (obj->kind != Ty_class) {
        error = TRUE;
        fprintf(out, "line %d:%d: '%s' is not a class type\n", e->pos->line, e->pos->pos, e->u.classvar.obj->u.v);
        return NULL;
    }
    S_table class = S_look(classTable, S_Symbol(obj->id));
    S_table var = S_look(class, S_Symbol(var_str));
    Ty_ty ty = S_look(var, S_Symbol(e->u.classvar.var));
    if (ty == NULL) {
        error = TRUE;
        fprintf(out, "line %d:%d: variable '%s' is not in class '%s'\n", e->pos->line, e->pos->pos, e->u.classvar.var, obj->id);
        return NULL;
    }
    return ty;
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
    S_table local = S_look(t, S_Symbol(local_str));
    Ty_ty ty = (Ty_ty)S_look(local, S_Symbol(e->u.v));
    if (ty == NULL) {
        error = TRUE;
        fprintf(out, "line: %d:%d: variable '%s' is not defined\n", e->pos->line, e->pos->pos, e->u.v);
        return NULL;
    }
    return ty;
}

static Ty_ty check_ThisExp(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    if (t->id == NULL) {
        error = TRUE;
        fprintf(out, "line: %d:%d: thisExp should be not in main method\n", e->pos->line, e->pos->pos);
        return NULL;
    }
    return Ty_Class(t->id);
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

static Ty_ty check_NewObjExp(FILE* out, S_table t, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    if (S_look(classTable, S_Symbol(e->u.v)) == NULL) {
        error = TRUE;
        fprintf(out, "line: %d:%d: class '%s' is not defined\n", e->pos->line, e->pos->pos, e->u.v);
        return NULL;
    }
    return Ty_Class(e->u.v);
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
    if (ty->kind != Ty_array) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int array is expected in getarray()\n", e->u.e->pos->line, e->u.e->pos->pos);        
        return NULL;        
    }
    if (!ty->pointer) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int pointer is expected in getarray()\n", e->u.e->pos->line, e->u.e->pos->pos);        
        return NULL;
    }

    return Ty_Int();
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

static bool check_Class(Ty_ty fa, Ty_ty son) {
    while (strcmp(son->id, fa->id)) {
        classNode node = S_look(inheritTable, S_Symbol(son->id));
        if (node == NULL) {
            return FALSE;
        }
        if (node->fa == NULL) {
            return FALSE;
        }
        son = Ty_Class(node->fa->id);
    }

    return TRUE;
}

static bool check_FuncExpList(FILE* out, S_table t, A_expList el, Ty_tyList fl) {
    if (!el && !fl) {
        return TRUE;
    } else if (!el || !fl) {
        return FALSE;
    }
    Ty_ty ty = check_Exp(out, t, el->head);
    if (error) {
        return FALSE;
    }
    if (ty->kind != fl->head->kind) {
        return FALSE;
    } else if (ty->kind == Ty_class) {
        if (check_Class(fl->head, ty) == FALSE) {
            return FALSE;
        }
    }
    return check_FuncExpList(out, t, el->tail, fl->tail);
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
    case A_idType: {
        if (S_look(classTable, S_Symbol(vd->t->id)) == NULL) {
            error = TRUE;
            fprintf(out, "line %d:%d: class '%s' is not defined\n", vd->pos->line, vd->pos->pos, vd->t->id);
            return;
        }
        S_enter(t, S_Symbol(vd->v), Ty_Class(vd->t->id)); 
        break;
    }
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
        fprintf(out, "line %d:%d: a location type is expected on the left of AssignStm\n", s->u.assign.arr->pos->line, s->u.assign.arr->pos->pos);
        return;        
    }
    if (value->kind != arr->kind) {
        error = TRUE;
        if (arr->kind == Ty_int) {
            fprintf(out, "line %d:%d: an int type is expected on the right of AssignStm\n", s->u.assign.value->pos->line, s->u.assign.value->pos->pos);
        } else if (arr->kind == Ty_array) {
            fprintf(out, "line %d:%d: an array type is expected on the right of AssignStm\n", s->u.assign.value->pos->line, s->u.assign.value->pos->pos);
        }
        return;               
    } else if (value->kind == Ty_class && check_Class(arr, value) == FALSE) {
        error = TRUE;
        fprintf(out, "line %d:%d: a class '%s' type is expected on the right of AssignStm\n", s->u.assign.value->pos->line, s->u.assign.value->pos->pos, arr->id);
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
        fprintf(out, "line %d:%d: an array type is expected on the left of AssignStm\n", s->u.array_init.arr->pos->line, s->u.array_init.arr->pos->pos);
        return;        
    }
    if (!arr->location) {
        error = TRUE;
        fprintf(out, "line %d:%d: an location type is expected on the left of AssignStm\n", s->u.array_init.arr->pos->line, s->u.array_init.arr->pos->pos);
        return; 
    }
    check_ArrayExpList(out, t, s->u.array_init.init_values);
}

static void check_CallStm(FILE* out, S_table t, A_stm s) {
    if (!s || error) {
        return;
    }
    Ty_ty obj = check_Exp(out, t, s->u.call_stat.obj);
    if (obj->kind != Ty_class) {
        error = TRUE;
        fprintf(out, "line %d:%d: '%s' is not a class type\n", s->pos->line, s->pos->pos, s->u.call_stat.obj->u.v);
        return;
    }
    S_table class = S_look(classTable, S_Symbol(obj->id));
    S_table method = S_look(class, S_Symbol(method_str));
    Ty_ty fun = S_look(method, S_Symbol(s->u.call_stat.fun));
    if (fun == NULL) {
        error = TRUE;
        fprintf(out, "line %d:%d: class '%s' doesn't have function %s\n", s->pos->line, s->pos->pos, obj->id, s->u.call_stat.fun);
        return;
    }
    if (check_FuncExpList(out, t, s->u.call_stat.el, fun->fl) == FALSE) {
        if (error) {
            return;
        }
        error = TRUE;
        fprintf(out, "line %d:%d: argument type error", s->pos->line, s->pos->pos);
        return;
    }
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
    S_table local = S_look(t, S_Symbol(local_str));
    if (error || ty == NULL) {
        return;
    }
    if (ty->kind != local->ret->kind) {
        error = TRUE;
        fprintf(out, "line %d:%d: return type error\n", s->u.e->pos->line, s->u.e->pos->pos);        
        return;
    } else if (ty->kind == Ty_class && check_Class(local->ret, ty) == FALSE) {
        error = TRUE;
        fprintf(out, "line %d:%d: return type error\n", s->u.e->pos->line, s->u.e->pos->pos);        
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
    S_table local = S_empty();
    S_enter(t, S_Symbol(local_str), local);
    local->ret = Ty_Int();
    if (main->vdl) {
        check_VarDeclList(out, local, main->vdl);
    }
    if (main->sl) {
        check_StmList(out, t, main->sl);
    }
}

static void check_FormalList(FILE* out, S_table t, A_formalList fl) {
    if (!fl || error) {
        return;
    }
    Ty_ty ty;
    switch (fl->head->t->t) {
    case A_intType: ty = Ty_LocationInt(); break;
    case A_idType: {
        if (S_look(classTable, S_Symbol(fl->head->t->id)) == NULL) {
            error = TRUE;
            fprintf(out, "line %d:%d: class '%s' is not defined\n", fl->head->pos->line, fl->head->pos->pos, fl->head->t->id);
            return;
        }
        ty = Ty_Class(fl->head->t->id);
        break;
    }
    case A_intArrType: ty = Ty_LocationPointerArray(); break;
    }
    S_enter(t, S_Symbol(fl->head->id), ty);

    if (fl->tail) {
        check_FormalList(out, t, fl->tail);
    }
}

static void check_MethodDecl(FILE* out, S_table t, A_methodDecl md) {
    if (!md || error) {
        return;
    }
    S_table local = S_look(t, S_Symbol(local_str));
    S_table method = S_look(t, S_Symbol(method_str));
    Ty_ty fun = S_look(method, S_Symbol(md->id));
    local->ret = fun->ret;
    S_beginScope(local);
    if (md->fl) {
        check_FormalList(out, local, md->fl);
    }
    if (md->vdl) {
        check_VarDeclList(out, local, md->vdl);
    }
    if (md->sl) {
        check_StmList(out, t, md->sl);
    }
    S_endScope(local);
}

static void check_MethodDeclList(FILE* out, S_table t, A_methodDeclList mdl) {
    if (!mdl || error) {
        return;
    }
    check_MethodDecl(out, t, mdl->head);
    if (mdl->tail) {
        check_MethodDeclList(out, t, mdl->tail);
    }
}

static Ty_tyList Ty_TyList(FILE* out, A_formalList fl) {
    if (!fl || error) {
        return NULL;
    }
    Ty_tyList p = checked_malloc(sizeof(*p));
    switch (fl->head->t->t) {
    case A_intType: p->head = Ty_Int(); break;
    case A_intArrType: p->head = Ty_Array(); break;
    case A_idType: {
        p->head = Ty_Class(fl->head->t->id); 
        if (S_look(classTable, S_Symbol(fl->head->t->id)) == NULL) {
            error = TRUE;
            fprintf(out, "line %d:%d: class '%s' is not defined\n", fl->head->pos->line, fl->head->pos->pos, fl->head->t->id);
            return NULL;
        }
        break;
    }
    default: break;
    }

    p->tail = NULL;
    if (fl->tail) {
        p->tail = Ty_TyList(out, fl->tail);
    }
    return p;
}

static bool check_TyList(Ty_tyList ty1, Ty_tyList ty2) {
    if (!ty1 && !ty2) {
        return TRUE;
    } else if (!ty1 || !ty2) {
        return FALSE;
    }
    if (ty1->head->kind != ty2->head->kind) {
        return FALSE;
    } else if (ty1->head->kind == Ty_class && ty1->head->id != ty2->head->id) {
        return FALSE;
    }

    return check_TyList(ty1->tail, ty2->tail);
}

static void check_MethodDeclList_wrap(FILE* out, S_table t, A_methodDeclList mdl) {
    // 先遍历一遍获得方法签名
    if (!mdl || error) {
        return;
    }
    A_methodDeclList cur = mdl;
    S_table method = S_look(t, S_Symbol(method_str));
    S_table temp = S_empty();
    while (cur) {
        A_methodDecl md = cur->head;
        Ty_ty ret = NULL;
        switch (md->t->t) {
        case A_intType: ret = Ty_Int(); break;
        case A_intArrType: ret = Ty_Array(); break;
        case A_idType: {
            ret = Ty_Class(md->t->id);
            if (S_look(classTable, S_Symbol(md->t->id)) == NULL) {
                error = TRUE;
                fprintf(out, "line %d:%d: class '%s' is not defined\n", md->pos->line, md->pos->pos, md->t->id);
                return;
            }
            break;
        }
        default: break;
        }
        Ty_tyList fl = Ty_TyList(out, md->fl);
        if (error) {
            return;
        }

        Ty_ty ty = Ty_Method(md->id, fl, ret);
        Ty_ty old_ty = S_look(method, S_Symbol(md->id));
        // 继承
        if (old_ty != NULL) {
            if (old_ty->ret->kind != ty->ret->kind) {
                error = TRUE;
            } else if (old_ty->ret->kind == Ty_class && old_ty->ret->id != ty->ret->id) {
                error = TRUE;
            }
            if (check_TyList(old_ty->fl, ty->fl) == FALSE) {
                error = TRUE;
            }
        }
        if (error) {
            fprintf(out, "line %d:%d: illegal override\n", md->pos->line, md->pos->pos);
            return;
        }
        if (S_look(temp, S_Symbol(md->id)) != NULL) {
            error = TRUE;
            fprintf(out, "line %d:%d: method %s redefinition\n", md->pos->line, md->pos->pos, md->id);
            return;
        }
        S_enter(temp, S_Symbol(md->id), ty);
        cur = cur->tail;
    }
    S_copy(temp, method);
    // 再检查方法内部
    check_MethodDeclList(out, t, mdl);
}

static void parse_ClassDeclListWithExtends(FILE* out, S_table t, A_classDeclList cdl) {
    if (!cdl || error) {
        return;
    }
    A_classDeclList head = cdl;
    // 第一遍获得类名和继承关系
    while (cdl) {
        A_classDecl cd = cdl->head;
        if (S_look(t, S_Symbol(cd->id)) != NULL) {
            error = TRUE;
            fprintf(out, "line %d:%d: class '%s' redefinition\n", cd->pos->line, cd->pos->pos, cd->id);
            return;
        }
        S_table class = S_empty();
        class->id = cd->id;
        S_enter(t, S_Symbol(cd->id), class);
        S_enter(inheritTable, S_Symbol(cd->id), ClassNode(cd, NULL, 0)); 
        cdl = cdl->tail;
    }

    // 第二遍构建继承关系
    cdl = head;
    while (cdl) {
        A_classDecl cd = cdl->head;
        while (cd->parentID != NULL) {
            classNode node = S_look(inheritTable, S_Symbol(cd->id));
            if (node->vis == 1) {
                break;
            }
            node->vis = 1;
            classNode parent = S_look(inheritTable, S_Symbol(cd->parentID));
            if (parent == NULL) {
                error = TRUE;
                fprintf(out, "line %d:%d: class %s's parent class %s is not defined\n", cd->pos->line, cd->pos->pos, cd->id, cd->parentID);
                return;
            }
            node->fa = parent->base;
            cd = node->fa;
            // 发现继承环
            if (cd == cdl->head) {
                error = TRUE;
                fprintf(out, "line %d:%d: circular inheritance %s", cd->pos->line, cd->pos->pos, cd->id);
                node = S_look(inheritTable, S_Symbol(cd->id));
                cd = node->fa;
                while (cd != cdl->head) {
                    fprintf(out, " => %s", cd->id);
                    node = S_look(inheritTable, S_Symbol(cd->id));
                    cd = node->fa;
                }
                fprintf(out, " => %s", cd->id);
                return;
            }
        }
        cdl = cdl->tail;
    }
}

static void parse_ClassDecl(FILE* out, S_table t, A_classDecl cd) {
    if (!cd || error) {
        return;
    }
    classNode node = S_look(inheritTable, S_Symbol(cd->id));
    if (node->vis == 2) {
        return;
    }
    node->vis = 2;

    // 创建方法表和变量表
    S_table method = S_empty();
    S_table var = S_empty();
    S_table local = S_empty();
    S_enter(t, S_Symbol(method_str), method);
    S_enter(t, S_Symbol(var_str), var);
    S_enter(t, S_Symbol(local_str), local);
    // 先解析父类
    if (cd->parentID) {
        classNode parent = S_look(inheritTable, S_Symbol(cd->parentID));
        S_table parentTable = S_look(classTable, S_Symbol(cd->parentID));
        parse_ClassDecl(out, parentTable, parent->base);
        if (error) {
            return;
        }
        // 复制父类的表
        S_copy(S_look(parentTable, S_Symbol(method_str)), method);
        S_copy(S_look(parentTable, S_Symbol(var_str)), var);    
    }
    // 再解析自身
    check_VarDeclList(out, var, cd->vdl);
    check_MethodDeclList_wrap(out, t, cd->mdl);
}

static void parse_ClassDeclList(FILE* out, S_table t, A_classDeclList cdl) {
    if (!cdl || error) {
        return;
    }
    parse_ClassDecl(out, S_look(t, S_Symbol(cdl->head->id)), cdl->head);
    if (cdl->tail) {
        parse_ClassDeclList(out, t, cdl->tail);
    }
}

static void check_ClassDeclList(FILE* out, S_table t, A_classDeclList cdl) {
    if (!cdl || error) {
        return;
    }
    // 第一遍扫描仅解析类名和继承关系
    parse_ClassDeclListWithExtends(out, t, cdl);
    // 第二遍解析类内部
    parse_ClassDeclList(out, t, cdl);
    return;
}

void check_Prog(FILE* out, A_prog p) {
    classTable = S_empty();
    mainTable = S_empty();
    inheritTable = S_empty();
    if (p->cdl) {
        check_ClassDeclList(out, classTable, p->cdl);
    }
    if (error) {
        return;
    }
    if (p->m) {
        check_MainMethod(out, mainTable, p->m);
    } else {
        fprintf(out, "Error: There's no main class!\n");
        error = TRUE;
    }
}