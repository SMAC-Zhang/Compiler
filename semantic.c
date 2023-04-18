#include <string.h>
#include "semantic.h"
#include "symbol.h"
#include "types.h"

static bool error;
S_table classTable;
static S_table inheritTable;

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

classEntry ClassEntry(string id) {
    classEntry ce = checked_malloc(sizeof *ce);
    memset(ce, 0, sizeof *ce);
    ce->methodTable = S_empty();
    ce->varTable = S_empty();
    ce->id = id;
    ce->var_offset = 0;
    ce->method_offset = 0;
    return ce;
}

methodEntry MethodEntry(classEntry ce, classEntry from, Ty_ty ret, Ty_tyList fl, A_methodDecl md, int offset) {
    methodEntry me = checked_malloc(sizeof *me);
    memset(me, 0, sizeof *me);
    me->localTable = S_empty();
    me->tempTable = S_empty();
    me->ce = ce;
    me->from = from;
    me->ret = ret;
    me->fl = fl;
    me->offset = offset;
    me->md = md;
    return me;
}

varEntry VarEntry(Ty_ty ty, A_varDecl vd, int offset) {
    varEntry ve = checked_malloc(sizeof *ve);
    memset(ve, 0, sizeof *ve);
    ve->ty = ty;
    ve->vd = vd;
    ve->offset = offset;
    return ve;
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

static bool check_FuncExpList(FILE* out, methodEntry me, A_expList el, Ty_tyList fl) {
    if (!el && !fl) {
        return TRUE;
    } else if (!el || !fl) {
        return FALSE;
    }
    Ty_ty ty = check_Exp(out, me, el->head);
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
    return check_FuncExpList(out, me, el->tail, fl->tail);
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

static void check_ArrayExpList(FILE* out, methodEntry me, A_expList el) {
    if (!el || error) {
        return;
    }
    Ty_ty ty = check_Exp(out, me, el->head);
    if (error) {
        return;
    }
    if (ty != NULL && ty->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected in array initialization list\n", el->head->pos->line, el->head->pos->pos);        
        return;
    }
    if (el->tail) {
        check_ArrayExpList(out, me, el->tail);
    }
}

static Ty_tyList Fl_To_Ty_TyList(FILE* out, A_formalList fl) {
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
        p->tail = Fl_To_Ty_TyList(out, fl->tail);
    }
    return p;
}

Ty_ty check_OpExp(FILE* out, methodEntry me, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty left = check_Exp(out, me, e->u.op.left);
    Ty_ty right = check_Exp(out, me, e->u.op.right);
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

Ty_ty check_ArrayExp(FILE* out, methodEntry me, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty arr = check_Exp(out, me, e->u.array_pos.arr);
    Ty_ty arr_pos = check_Exp(out, me, e->u.array_pos.arr_pos);
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

Ty_ty check_CallExp(FILE* out, methodEntry me, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty obj = check_Exp(out, me, e->u.call.obj);
    if (obj->kind != Ty_class) {
        error = TRUE;
        fprintf(out, "line %d:%d: '%s' is not a class type\n", e->pos->line, e->pos->pos, e->u.call.obj->u.v);
        return NULL;
    }
    classEntry ce = (classEntry)S_look(classTable, S_Symbol(obj->id));
    S_table methodTable = ce->methodTable;
    methodEntry localme = S_look(methodTable, S_Symbol(e->u.call.fun));
    if (localme == NULL) {
        error = TRUE;
        fprintf(out, "line %d:%d: class '%s' doesn't have function %s\n", e->pos->line, e->pos->pos, obj->id, e->u.call.fun);
        return NULL;
    }
    if (check_FuncExpList(out, me, e->u.call.el, localme->fl) == FALSE) {
        if (error) {
            return NULL;
        }
        error = TRUE;
        fprintf(out, "line %d:%d: argument type error", e->pos->line, e->pos->pos);
        return NULL;
    }
    return localme->ret;
}

Ty_ty check_ClassVarExp(FILE* out, methodEntry me, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty obj = check_Exp(out, me, e->u.classvar.obj);
    if (error || obj == NULL) {
        return NULL;
    }
    if (obj->kind != Ty_class) {
        error = TRUE;
        fprintf(out, "line %d:%d: '%s' is not a class type\n", e->pos->line, e->pos->pos, e->u.classvar.obj->u.v);
        return NULL;
    }
    classEntry ce = (classEntry)S_look(classTable, S_Symbol(obj->id));
    S_table varTable = ce->varTable;
    Ty_ty ty = ((varEntry)S_look(varTable, S_Symbol(e->u.classvar.var)))->ty;
    if (ty == NULL) {
        error = TRUE;
        fprintf(out, "line %d:%d: variable '%s' is not in class '%s'\n", e->pos->line, e->pos->pos, e->u.classvar.var, obj->id);
        return NULL;
    }
    return ty;
}

Ty_ty check_BoolConst(FILE* out, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    return Ty_Int();
}

Ty_ty check_NumConst(FILE* out, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    return Ty_Int();
}

Ty_ty check_LengthExp(FILE* out, methodEntry me, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty ty = check_Exp(out, me, e->u.e);
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

Ty_ty check_IdExp(FILE* out, methodEntry me, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    S_table localTable = me->localTable;
    varEntry ve = (varEntry)S_look(localTable, S_Symbol(e->u.v));
    if (ve == NULL) {
        error = TRUE;
        fprintf(out, "line: %d:%d: variable '%s' is not defined\n", e->pos->line, e->pos->pos, e->u.v);
        return NULL;
    }
    return ve->ty;
}

Ty_ty check_ThisExp(FILE* out, methodEntry me, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    if (me->ce == NULL) {
        error = TRUE;
        fprintf(out, "line: %d:%d: thisExp should be not in main method\n", e->pos->line, e->pos->pos);
        return NULL;
    }
    return Ty_Class(me->ce->id);
}

Ty_ty check_NewIntArrExp(FILE* out, methodEntry me, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty ty = check_Exp(out, me, e->u.e);
    if (ty->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected as new array's size\n", e->u.e->pos->line, e->u.e->pos->pos);        
        return NULL;
    }
    return Ty_PointerArray();
}

Ty_ty check_NewObjExp(FILE* out, methodEntry me, A_exp e) {
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

Ty_ty check_NotExp(FILE* out, methodEntry me, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty ty = check_Exp(out, me, e->u.e);
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

Ty_ty check_MinusExp(FILE* out, methodEntry me, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty ty = check_Exp(out, me, e->u.e);
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

Ty_ty check_EscExp(FILE* out, methodEntry me, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    check_StmList(out, me, e->u.escExp.ns);
    if (error) {
        return NULL;
    }
    Ty_ty ty = check_Exp(out, me, e->u.escExp.exp);
    if (error || ty == NULL) {
        return NULL;
    }
    return ty;
}

Ty_ty check_Getint(FILE* out, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    return Ty_Int();
}

Ty_ty check_Getch(FILE* out, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    return Ty_Int();
}

Ty_ty check_Getarray(FILE* out, methodEntry me, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    Ty_ty ty = check_Exp(out, me, e->u.e);
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

Ty_ty check_Exp(FILE* out, methodEntry me, A_exp e) {
    if (!e || error) {
        return NULL;
    }
    switch (e->kind) {
    case A_opExp: return check_OpExp(out, me, e); break;
    case A_arrayExp: return check_ArrayExp(out, me, e); break;
    case A_callExp: return check_CallExp(out, me, e); break;
    case A_classVarExp: return check_ClassVarExp(out, me, e); break;
    case A_boolConst: return check_BoolConst(out, e); break;
    case A_numConst: return check_NumConst(out, e); break;
    case A_lengthExp: return check_LengthExp(out, me, e); break;
    case A_idExp: return check_IdExp(out, me, e); break;
    case A_thisExp: return check_ThisExp(out, me, e); break;
    case A_newIntArrExp: return check_NewIntArrExp(out, me, e); break;
    case A_newObjExp: return check_NewObjExp(out, me, e); break;
    case A_notExp: return check_NotExp(out, me, e); break;
    case A_minusExp: return check_MinusExp(out, me, e); break;
    case A_escExp: return check_EscExp(out, me, e); break;
    case A_getint: return check_Getint(out, e); break;
    case A_getch: return check_Getch(out, e); break;
    case A_getarray: return check_Getarray(out, me, e); break;
    default: error = TRUE; fprintf(out, "line %d:%d: Unknown expression kind!\n", e->pos->line, e->pos->pos); break;
    }
}

void check_VarDecl(FILE* out, methodEntry me, S_table t, A_varDecl vd, int offset) {
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
    case A_intType: S_enter(t, S_Symbol(vd->v), VarEntry(Ty_LocationInt(), vd, offset)); break;
    case A_idType: {
        if (S_look(classTable, S_Symbol(vd->t->id)) == NULL) {
            error = TRUE;
            fprintf(out, "line %d:%d: class '%s' is not defined\n", vd->pos->line, vd->pos->pos, vd->t->id);
            return;
        }
        S_enter(t, S_Symbol(vd->v), VarEntry(Ty_Class(vd->t->id), vd, offset)); 
        break;
    }
    case A_intArrType: S_enter(t, S_Symbol(vd->v), VarEntry(Ty_LocationPointerArray(), vd, offset)); break;
    }
    
    // definition
    if (vd->elist) {
        if (vd->t->t == A_intType) {
            Ty_ty ty = check_Exp(out, me, vd->elist->head);
            if (error || ty == NULL) {
                return;
            }
            if(ty->kind != Ty_int) {
                error = TRUE;
                fprintf(out, "line: %d:%d: assign error: an int type is expected\n", vd->pos->line, vd->pos->pos);
            }
        } else if (vd->t->t == A_intArrType) {
            check_ArrayExpList(out, me, vd->elist);
            if (error) {
                return;
            }
        } else {
            fprintf(out, "line: %d:%d: Initialize non array?!\n");
        }
    }
}

void check_VarDeclList(FILE* out, methodEntry me, classEntry ce, S_table t, A_varDeclList vdl, int offset) {
    if (!vdl || error) {
        return;
    }
    check_VarDecl(out, me, t, vdl->head, offset);
    if (vdl->tail) {
        check_VarDeclList(out, me, ce, t, vdl->tail, offset + 1);
    } else {
        if (ce) {
            ce->var_offset = offset + 1;
        }
    }
}

void check_NestedStm(FILE* out, methodEntry me, A_stm s) {
    if (!s || error) {
        return;
    }
    check_StmList(out, me, s->u.ns);
}

void check_IfStm(FILE* out, methodEntry me, A_stm s) {
    if (!s || error) {
        return;
    }
    Ty_ty ty = check_Exp(out, me, s->u.if_stat.e);
    if (error || ty == NULL) {
        return;
    }
    if (ty->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected in if()\n", s->u.if_stat.e->pos->line, s->u.if_stat.e->pos->pos);        
        return;
    }
    check_Stm(out, me, s->u.if_stat.s1);
    if (s->u.if_stat.s2) {
        check_Stm(out, me, s->u.if_stat.s2);
    }
}

void check_WhileStm(FILE* out, methodEntry me, A_stm s) {
    if (!s || error) {
        return;
    }
    Ty_ty ty = check_Exp(out, me, s->u.while_stat.e);
    if (error || ty == NULL) {
        return;
    }
    if (ty->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected in while()\n", s->u.while_stat.e->pos->line, s->u.while_stat.e->pos->pos);
        return;
    }
    if (s->u.while_stat.e) {
        check_Stm(out, me, s->u.while_stat.s);
    }
}

void check_AssignStm(FILE* out, methodEntry me, A_stm s) {
    if (!s || error) {
        return;
    }
    Ty_ty arr = check_Exp(out, me, s->u.assign.arr);
    Ty_ty value = check_Exp(out, me, s->u.assign.value);
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

void check_ArrayInit(FILE* out, methodEntry me, A_stm s) {
    if (!s || error) {
        return;
    }
    Ty_ty arr = check_Exp(out, me, s->u.array_init.arr);
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
    check_ArrayExpList(out, me, s->u.array_init.init_values);
}

void check_CallStm(FILE* out, methodEntry me, A_stm s) {
    if (!s || error) {
        return;
    }
    Ty_ty obj = check_Exp(out, me, s->u.call_stat.obj);
    if (obj->kind != Ty_class) {
        error = TRUE;
        fprintf(out, "line %d:%d: '%s' is not a class type\n", s->pos->line, s->pos->pos, s->u.call_stat.obj->u.v);
        return;
    }
    classEntry ce = (classEntry)S_look(classTable, S_Symbol(obj->id));
    S_table methodTable = ce->methodTable;
    methodEntry localme = S_look(methodTable, S_Symbol(s->u.call_stat.fun));
    if (localme == NULL) {
        error = TRUE;
        fprintf(out, "line %d:%d: class '%s' doesn't have function %s\n", s->pos->line, s->pos->pos, obj->id, s->u.call_stat.fun);
        return;
    }
    if (check_FuncExpList(out, me, s->u.call_stat.el, localme->fl) == FALSE) {
        if (error) {
            return;
        }
        error = TRUE;
        fprintf(out, "line %d:%d: argument type error", s->pos->line, s->pos->pos);
        return;
    }
    return;
}

void check_Putint(FILE* out, methodEntry me, A_stm s) {
    if (!s || error) {
        return;
    }
    Ty_ty ty = check_Exp(out, me, s->u.e);
    if (error || ty == NULL) {
        return;
    }
    if (ty->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected in putint()\n", s->u.e->pos->line, s->u.e->pos->pos);        
        return;
    }
}

void check_Putch(FILE* out, methodEntry me, A_stm s) {
    if (!s || error) {
        return;
    }
    Ty_ty ty = check_Exp(out, me, s->u.e);
    if (error || ty == NULL) {
        return;
    }
    if (ty->kind != Ty_int) {
        error = TRUE;
        fprintf(out, "line %d:%d: an int type is expected in putch()\n", s->u.e->pos->line, s->u.e->pos->pos);        
        return;
    }
}

void check_Putarray(FILE* out, methodEntry me, A_stm s) {
    if (!s || error) {
        return;
    }
    Ty_ty ty1 = check_Exp(out, me, s->u.putarray.e1);
    Ty_ty ty2 = check_Exp(out, me, s->u.putarray.e2);
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

void check_Return(FILE* out, methodEntry me, A_stm s) {
    if (!s || error) {
        return;
    }
    Ty_ty ty = check_Exp(out, me, s->u.e);
    if (error || ty == NULL) {
        return;
    }
    if (ty->kind != me->ret->kind) {
        error = TRUE;
        fprintf(out, "line %d:%d: return type error\n", s->u.e->pos->line, s->u.e->pos->pos);        
        return;
    } else if (ty->kind == Ty_class && check_Class(me->ret, ty) == FALSE) {
        error = TRUE;
        fprintf(out, "line %d:%d: return type error\n", s->u.e->pos->line, s->u.e->pos->pos);        
        return;
    }
}

void check_Stm(FILE* out, methodEntry me, A_stm s) {
    if (!s || error) {
        return;
    }
    switch (s->kind) {
    case A_nestedStm: check_NestedStm(out, me, s); break;
    case A_ifStm: check_IfStm(out, me, s); break;
    case A_whileStm: check_WhileStm(out, me, s); break;
    case A_assignStm: check_AssignStm(out, me, s); break;
    case A_arrayInit: check_ArrayInit(out, me, s); break;
    case A_callStm: check_CallStm(out, me, s); break;
    case A_continue: break;
    case A_break: break;
    case A_return: check_Return(out, me, s); break;
    case A_putint: check_Putint(out, me, s); break;
    case A_putarray: check_Putarray(out, me, s); break;
    case A_putch: check_Putch(out, me, s); break;
    case A_starttime: break;
    case A_stoptime: break;
    default: error = TRUE; fprintf(out, "line %d:%d: Unknown statement kind!\n", s->pos->line, s->pos->pos); break;
    }
}

void check_StmList(FILE* out, methodEntry me, A_stmList sl) {
    if (!sl || error) {
        return;
    }
    check_Stm(out, me, sl->head);
    if (sl->tail) {
        check_StmList(out, me, sl->tail);
    }
}

void check_MainMethod(FILE* out, methodEntry me, A_mainMethod main) {
    if (main->vdl) {
        check_VarDeclList(out, me, NULL, me->localTable, main->vdl, 0);
    }
    if (main->sl) {
        check_StmList(out, me, main->sl);
    }
}

void check_FormalList(FILE* out, S_table t, A_formalList fl) {
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
    if (S_look(t, S_Symbol(fl->head->id)) != NULL) {
        error = TRUE;
        fprintf(out, "line %d:%d: '%s' variable redefinition\n", fl->head->pos->line, fl->head->pos->pos, fl->head->id);
        return;
    } 
    S_enter(t, S_Symbol(fl->head->id), VarEntry(ty, NULL, 0));

    if (fl->tail) {
        check_FormalList(out, t, fl->tail);
    }
}

void check_MethodDecl(FILE* out, S_table t, A_methodDecl md) {
    if (!md || error) {
        return;
    }
    methodEntry me = S_look(t, S_Symbol(md->id));
    if (md->fl) {
        check_FormalList(out, me->localTable, md->fl);
    }
    if (md->vdl) {
        check_VarDeclList(out, me, NULL, me->localTable, md->vdl, 0);
    }
    if (md->sl) {
        check_StmList(out, me, md->sl);
    }
}

void check_MethodDeclList(FILE* out, S_table t, A_methodDeclList mdl) {
    if (!mdl || error) {
        return;
    }
    check_MethodDecl(out, t, mdl->head);
    if (mdl->tail) {
        check_MethodDeclList(out, t, mdl->tail);
    }
}

void check_MethodDeclList_wrap(FILE* out, classEntry ce, A_methodDeclList mdl) {
    // 先遍历一遍获得方法签名
    if (!mdl || error) {
        return;
    }
    S_table methodTable = ce->methodTable;
    A_methodDeclList cur = mdl;
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
        Ty_tyList fl = Fl_To_Ty_TyList(out, md->fl);
        if (error) {
            return;
        }

        methodEntry new_method = MethodEntry(ce, ce, ret, fl, md, ce->method_offset++);
        methodEntry old_method = (methodEntry)S_look(methodTable, S_Symbol(md->id));
        // 继承
        if (old_method != NULL) {
            if (old_method->ret->kind != new_method->ret->kind) {
                error = TRUE;
            } else if (old_method->ret->kind == Ty_class && old_method->ret->id != new_method->ret->id) {
                error = TRUE;
            }
            if (check_TyList(old_method->fl, new_method->fl) == FALSE) {
                error = TRUE;
            }
            new_method->offset = old_method->offset;
            ce->method_offset--;
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
        S_enter(temp, S_Symbol(md->id), new_method);
        cur = cur->tail;
    }
    S_copy(temp, methodTable);
    // 再检查方法内部
    check_MethodDeclList(out, methodTable, mdl);
}

void parse_ClassDeclListWithExtends(FILE* out, S_table t, A_classDeclList cdl) {
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
        classEntry class = ClassEntry(cd->id);
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

void parse_ClassDecl(FILE* out, classEntry ce, A_classDecl cd) {
    if (!cd || error) {
        return;
    }
    classNode node = S_look(inheritTable, S_Symbol(cd->id));
    if (node->vis == 2) {
        return;
    }
    node->vis = 2;

    // 先解析父类
    if (cd->parentID) {
        classNode parent = S_look(inheritTable, S_Symbol(cd->parentID));
        classEntry parentCe = (classEntry)S_look(classTable, S_Symbol(cd->parentID));
        parse_ClassDecl(out, parentCe, parent->base);
        if (error) {
            return;
        }
        // 复制父类的表
        S_copy(parentCe->methodTable, ce->methodTable);
        S_copy(parentCe->varTable, ce->varTable);
        ce->var_offset = parentCe->var_offset;
        ce->method_offset = parentCe->method_offset;
    }
    // 再解析自身
    int diff = ce->var_offset;
    check_VarDeclList(out, NULL, ce, ce->varTable, cd->vdl, ce->var_offset);
    diff = ce->var_offset - diff;
    // 修改继承的methodEntry
    for (int i = 0; i < TABSIZE; ++i) {
        binder b = ce->methodTable->table[i];
        while (b) {
            ((methodEntry)b->value)->ce = ce;
            ((methodEntry)b->value)->offset += diff;
            b = b->next;
        }
    }
    ce->method_offset += diff;
    check_MethodDeclList_wrap(out, ce, cd->mdl);
}

void parse_ClassDeclList(FILE* out, S_table t, A_classDeclList cdl) {
    if (!cdl || error) {
        return;
    }
    parse_ClassDecl(out, (classEntry)S_look(t, S_Symbol(cdl->head->id)), cdl->head);
    if (cdl->tail) {
        parse_ClassDeclList(out, t, cdl->tail);
    }
}

void check_ClassDeclList(FILE* out, S_table t, A_classDeclList cdl) {
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
    methodEntry mainMethodEntry = MethodEntry(NULL, NULL, Ty_Int(), Ty_TyList(NULL, NULL), NULL, 0);
    inheritTable = S_empty();
    if (p->cdl) {
        check_ClassDeclList(out, classTable, p->cdl);
    }
    if (error) {
        return;
    }
    if (p->m) {
        check_MainMethod(out, mainMethodEntry, p->m);
    } else {
        fprintf(out, "Error: There's no main class!\n");
        error = TRUE;
    }
}