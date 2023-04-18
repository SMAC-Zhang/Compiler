#include <stdlib.h>
#include "translate.h"
#include "temp.h"
#include "symbol.h"
#include "semantic.h"

static int byte_length = 4; // 4 Bytes
extern S_table classTable;
extern methodEntry mainMethodEntry;

// patch 和 Cx
typedef struct patchList_ *patchList;
struct patchList_ {
    Temp_label *head;
    patchList tail;
};

static patchList PatchList(Temp_label *head, patchList tail) {
    patchList pl = checked_malloc(sizeof *pl);
    pl->head = head;
    pl->tail = tail;
    return pl;
}

static void doPatch(patchList tList, Temp_label label) {
    for (; tList; tList = tList->tail) {
        *(tList->head) = label;
    }
}

static patchList joinPatch(patchList first, patchList second) {
    if (!first) {
        return second;
    }
    patchList head = first;
    while (first->tail) {
        first = first->tail;
    }
    first->tail = second;
    return head;
}

struct Cx {
    patchList trues;
    patchList falses;
    T_stm stm;
};

struct Tr_exp_ {
    enum {Tr_ex, Tr_nx, Tr_cx} kind;
    union {
        T_exp ex;
        T_stm nx;
        struct Cx cx;
    } u;
};

static Tr_exp Tr_Ex(T_exp ex) {
    Tr_exp e = checked_malloc(sizeof *e);
    e->kind = Tr_ex;
    e->u.ex = ex;
    return e;
}

static Tr_exp Tr_Nx(T_stm nx) {
    Tr_exp e = checked_malloc(sizeof *e);
    e->kind = Tr_nx;
    e->u.nx = nx;
    return e;
}

static Tr_exp Tr_Cx(patchList trues, patchList falses, T_stm stm) {
    Tr_exp e = checked_malloc(sizeof *e);
    e->kind = Tr_cx;
    e->u.cx.trues = trues;
    e->u.cx.falses = falses;
    e->u.cx.stm = stm;
    return e;
}

static T_exp unEx(Tr_exp e) {
    switch (e->kind) {
    case Tr_ex:
        return e->u.ex;
    case Tr_cx: {
        Temp_temp r = Temp_newtemp();
        Temp_label t = Temp_newlabel(), f = Temp_newlabel();
        doPatch(e->u.cx.trues, t);
        doPatch(e->u.cx.falses, f);
        return T_Eseq(T_Move(T_Temp(r), T_Const(1)),
                T_Eseq(e->u.cx.stm,
                    T_Eseq(T_Label(f),
                        T_Eseq(T_Move(T_Temp(r), T_Const(0)),
                            T_Eseq(T_Label(t), T_Temp(r))))));
    }
    case Tr_nx:
        return T_Eseq(e->u.nx, T_Const(0)); 
    }
}

static T_stm unNx(Tr_exp e) {
	switch(e->kind) {
    case Tr_ex:
        return T_Exp(e->u.ex);
    case Tr_nx:
        return e->u.nx;
    case Tr_cx: {
        Temp_temp r = Temp_newtemp();
        Temp_label t = Temp_newlabel(), f = Temp_newlabel();
        doPatch(e->u.cx.trues, t);
        doPatch(e->u.cx.falses, f);
        return T_Exp(T_Eseq(T_Move(T_Temp(r), T_Const(1)),
                T_Eseq(e->u.cx.stm,
                    T_Eseq(T_Label(f), 
                        T_Eseq(T_Move(T_Temp(r), T_Const(0)),
                            T_Eseq(T_Label(t), T_Temp(r)))))));
    }
	}
}

static struct Cx unCx(Tr_exp e) {
	switch(e->kind) {
    case Tr_ex: {
        struct Cx cx;
        cx.stm = T_Cjump(T_ne, e->u.ex, T_Const(0), NULL, NULL);
        cx.trues = PatchList(&(cx.stm->u.CJUMP.true), NULL);
        cx.falses = PatchList(&(cx.stm->u.CJUMP.false), NULL);
        return cx;
    }
    case Tr_nx: {
        fprintf(stderr, "error in UnCx\n");
        exit(1);
    }
    case Tr_cx: return e->u.cx;
	}
}

// jumpStack
typedef struct jumpLabel {
    Temp_label begin;
    Temp_label end;
} jumpLabel;

typedef struct CallStack {
    jumpLabel* jl;
    struct CallStack* next;
} CallStack;
static CallStack* cs;

static void push_stack(Temp_label begin, Temp_label end) {
    jumpLabel* jl = checked_malloc(sizeof *jl);
    jl->begin = begin;
    jl->end = end;
    CallStack* local_cs = checked_malloc(sizeof *local_cs);
    local_cs->jl = jl;
    local_cs->next = cs;
    cs = local_cs;
}

static void pop_stack() {
    if (cs == NULL) {
        fprintf(stderr, "jumpStack Error!\n");
        exit(1);
    }
    cs = cs->next;
}

static bool stack_empty() {
    return cs == NULL;
}

static bool loop_stack() {
    if (cs != NULL) {
        if (cs->jl->begin == NULL) {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

static Tr_exp translate_ArrayExpList(FILE* out, methodEntry me, A_expList el, T_exp t) {
    int len = 0;
    A_expList head = el;
    while (head) {
        head = head->tail;
        len++;
    }
    Temp_temp tt = Temp_newtemp();
    head = el;
    T_stm s = T_Seq(T_Move(T_Temp(tt), t), 
    T_Seq(
        T_Move(T_Temp(tt), 
            T_Binop(T_plus,
                T_ExtCall(String("malloc"), 
                    T_ExpList(
                        T_Binop(T_mul, T_Const(len + 1), T_Const(byte_length)),
                            NULL)), 
                T_Binop(T_mul, T_Const(1), T_Const(byte_length)))), NULL));
    T_stm h = s->u.SEQ.right;
    T_stm off = T_Move(
        T_Mem(
            T_Binop(T_plus, T_Temp(tt), T_Binop(T_mul, T_Const(-1), T_Const(byte_length)))
        ),
        T_Const(len)
    );
    if (len != 0) {
        h->u.SEQ.right = T_Seq(off, NULL);
        h = h->u.SEQ.right;
    } else {
        h->u.SEQ.right = off;
    }

    for (int i = 0; i < len; ++i, el = el->tail) {
        Tr_exp e = translate_Exp(out, me, el->head);
        T_stm left = T_Move(
            T_Mem(
                T_Binop(T_plus, T_Temp(tt), T_Binop(T_mul, T_Const(i), T_Const(byte_length)))
            ), 
            unEx(e)
        );
        if (i == len - 1) {
            h->u.SEQ.right = left;
        } else {
            h->u.SEQ.right = T_Seq(left, NULL);
            h = h->u.SEQ.right;
        }
    }
    return Tr_Nx(s);
}

static T_expList transform_ExpList(FILE* out, methodEntry me, A_expList el) {
    if (!el) {
        return NULL;
    }
    T_exp head = unEx(translate_Exp(out, me, el->head));
    T_expList tail = NULL;
    if (el->tail) {
        tail = transform_ExpList(out, me, el->tail);
    }
    return T_ExpList(head, tail);
}

static char** get_offset(int num, classEntry ce) {
    char** ret = (char**)malloc(num * sizeof(char*));
    S_table varTable = ce->varTable;
    S_table methodTable = ce->methodTable;
    for (int i = 0; i < TABSIZE; ++i) {
        binder b = varTable->table[i];
        while (b) {
            varEntry ve = (varEntry)(b->value);
            ret[ve->offset] = ve->vd->v;
            b = b->next;
        }
    }
    for (int i = 0; i < TABSIZE; ++i) {
        binder b = methodTable->table[i];
        while (b) {
            methodEntry me = (methodEntry)(b->value);
            ret[me->offset] = me->md->id;
            b = b->next;
        }
    }
    return ret;
}

static T_stm new_obj_VarDecl(FILE* out, methodEntry me, A_varDecl vd, T_exp mem) {
    if (!vd) {
        return NULL;
    }
    if (vd->elist) {
        if (vd->t->t == A_intType) {
            return T_Move(mem, T_Const(vd->elist->head->u.num));
        } else if (vd->t->t == A_intArrType) {
            Temp_temp t = Temp_newtemp();
            return T_Move(mem, 
                T_Eseq(unNx(translate_ArrayExpList(out, me, vd->elist, T_Temp(t))), 
                        T_Temp(t)));
        }
    }
    return NULL;
}

static T_funcDeclList joinFuncDeclList(T_funcDeclList first, T_funcDeclList second) {
    if (!first) {
        return second;
    }
    T_funcDeclList head = first;
    while (first->tail) {
        first = first->tail;
    }
    first->tail = second;
    return head;
}


// translate
Tr_exp translate_OpExp(FILE* out, methodEntry me, A_exp e) {
    if (!e) {
        return NULL;
    }
    Tr_exp left = translate_Exp(out, me, e->u.op.left);
    Tr_exp right = translate_Exp(out, me, e->u.op.right);
    // 普通二元运算符:
    switch (e->u.op.oper) {
    case A_plus: return Tr_Ex(T_Binop(T_plus, unEx(left), unEx(right)));
    case A_minus: return Tr_Ex(T_Binop(T_minus, unEx(left), unEx(right)));
    case A_times: return Tr_Ex(T_Binop(T_mul, unEx(left), unEx(right)));
    case A_div: return Tr_Ex(T_Binop(T_div, unEx(left), unEx(right)));
    }
    // 条件表达式:
    if (e->u.op.oper != A_and && e->u.op.oper != A_or) {
        T_relOp op = 0;
        switch (e->u.op.oper) {
        case A_less: op = T_lt; break;
        case A_le: op = T_le; break;
        case A_greater: op = T_gt; break;
        case A_ge: op = T_ge; break;
        case A_eq: op = T_eq; break;
        case A_ne: op = T_ne; break;
        }
        T_stm cond = T_Cjump(op, unEx(left), unEx(right), NULL, NULL);
	    patchList trues = PatchList(&cond->u.CJUMP.true, NULL);
	    patchList falses = PatchList(&cond->u.CJUMP.false, NULL);
        return Tr_Cx(trues, falses, cond);
    }
    // 与, 或, 短路逻辑
    struct Cx leftC = unCx(left), rightC = unCx(right);
    T_stm cond = NULL;
    patchList trues = NULL, falses = NULL;
    if (e->u.op.oper == A_and) {
        Temp_label t = Temp_newlabel();
        doPatch(leftC.trues, t);
        cond = T_Seq(leftC.stm, T_Seq(T_Label(t), rightC.stm)); 
        trues = rightC.trues;
        falses = joinPatch(leftC.falses, rightC.falses);
    } else if (e->u.op.oper == A_or) {
        Temp_label f = Temp_newlabel();
        doPatch(leftC.falses, f);
        cond = T_Seq(leftC.stm, T_Seq(T_Label(f), rightC.stm));
        trues = joinPatch(leftC.trues, rightC.trues);
        falses = rightC.falses;
    } else {
        fprintf(out, "UNKNOWN OP kind\n");
        exit(1);
    }
    return Tr_Cx(trues, falses, cond);
}

Tr_exp translate_ArrayExp(FILE* out, methodEntry me, A_exp e) {
    if (!e) {
        return NULL;
    }
    Tr_exp arr = translate_Exp(out, me, e->u.array_pos.arr);
    Tr_exp arr_pos = translate_Exp(out, me, e->u.array_pos.arr_pos);
    return Tr_Ex(T_Mem(T_Binop(T_plus, unEx(arr), 
        T_Binop(T_mul, unEx(arr_pos), T_Const(byte_length)))));
}

Tr_exp translate_CallExp(FILE* out, methodEntry me, A_exp e) {
    if (!e) {
        return NULL;
    }
    Ty_ty ty = check_Exp(out, me, e->u.call.obj);
    classEntry ce = (classEntry)S_look(classTable, S_Symbol(ty->id));
    methodEntry localme = (methodEntry)S_look(ce->methodTable, S_Symbol(e->u.call.fun));
    Tr_exp obj = translate_Exp(out, me, e->u.call.obj);
    Temp_temp t = Temp_newtemp();
    return Tr_Ex(
        T_Eseq(T_Move(T_Temp(t), unEx(obj)),
            T_Call(e->u.call.fun, 
                T_Binop(T_plus, T_Temp(t), T_Binop(T_mul, T_Const(localme->offset), T_Const(byte_length))),
                T_ExpList(T_Temp(t), transform_ExpList(out, me, e->u.call.el)))));
}

Tr_exp translate_ClassVarExp(FILE* out, methodEntry me, A_exp e) {
    if (!e) {
        return NULL;
    }
    Ty_ty ty = check_Exp(out, me, e->u.classvar.obj);
    classEntry ce = (classEntry)S_look(classTable, S_Symbol(ty->id));
    Tr_exp obj = translate_Exp(out, me, e->u.classvar.obj);
    int offset = ((varEntry)S_look(ce->varTable, S_Symbol(e->u.classvar.var)))->offset;
    return Tr_Ex(T_Mem(
                T_Binop(T_plus, unEx(obj), T_Binop(T_mul, T_Const(offset), T_Const(byte_length)))));
}

Tr_exp translate_ThisExp(FILE* out, methodEntry me, A_exp e) {
    if (!e) {
        return NULL;
    }
    return Tr_Ex(T_Temp(me->this));
}

Tr_exp translate_NewIntArrExp(FILE* out, methodEntry me, A_exp e) {
    if (!e) {
        return NULL;
    }
    Temp_temp t = Temp_newtemp();
    Tr_exp exp = translate_Exp(out, me, e->u.e);
    Temp_temp tt = Temp_newtemp();

    return Tr_Ex(
        T_Eseq(T_Seq(T_Move(T_Temp(tt), unEx(exp)),
                T_Seq(
                    T_Move(T_Temp(t),
                        T_Binop(T_plus, 
                            T_ExtCall(String("malloc"), 
                            T_ExpList(T_Binop(T_mul, 
                                T_Binop(T_plus, T_Temp(tt), T_Const(1)), 
                                T_Const(byte_length)), NULL)), 
                            T_Const(1))), 
                    T_Move(T_Mem(T_Binop(T_plus, T_Temp(t), T_Binop(T_mul, T_Const(-1), T_Const(byte_length)))),
                            T_Temp(tt)))),
                T_Temp(t)));
}

static void eliminate_seq_null(T_stm s) {
    if (!s || s->kind != T_SEQ) {
        return;
    }
    T_stm prev = s;
    while (s->u.SEQ.right && s->u.SEQ.right->kind == T_SEQ) {
        s = s->u.SEQ.right;
        if (prev->u.SEQ.right != s) {
            prev = prev->u.SEQ.right;
        }
    }
    if (s->u.SEQ.right == NULL) {
        if (prev == s) {
            memcpy(prev, s->u.SEQ.left, sizeof(*s));
        } else {
            prev->u.SEQ.right = s->u.SEQ.left;
        }
    }
}

Tr_exp translate_NewObjExp(FILE* out, methodEntry me, A_exp e) {
    if (!e) {
        return NULL;
    }
    // 申请空间
    Temp_temp t = Temp_newtemp();
    classEntry ce = (classEntry)S_look(classTable, S_Symbol(e->u.v));
    T_stm s = T_Move(T_Temp(t), 
                T_ExtCall(String("malloc"), 
                T_ExpList(T_Binop(T_mul, T_Const(ce->method_offset), T_Const(byte_length)), NULL)));
    char** off = get_offset(ce->method_offset, ce);
    T_stm ret = T_Seq(s, NULL);
    T_stm head = ret;
    // 填充空间
    for (int i = 0; i < ce->method_offset; ++i) {
        if (i < ce->var_offset) {
            varEntry ve = (varEntry)S_look(ce->varTable, S_Symbol(off[i]));
            T_exp mem = T_Mem(T_Binop(T_plus, T_Temp(t), 
                                T_Binop(T_mul, T_Const(i), T_Const(byte_length))));
            T_stm s = new_obj_VarDecl(out, me, ve->vd, mem);
            if (s != NULL) {
                T_stm seq = T_Seq(s, NULL);
                head->u.SEQ.right = seq;
                head = head->u.SEQ.right;
            }
        } else {
            methodEntry me = (methodEntry)S_look(ce->methodTable, S_Symbol(off[i]));
            T_exp mem = T_Mem(T_Binop(T_plus, T_Temp(t), 
                                T_Binop(T_mul, T_Const(i), T_Const(byte_length))));
            char* id = (char*)malloc(strlen(me->from->id) + strlen(me->md->id) + 2);
            sprintf(id, "%s%c%s", me->from->id, '_', me->md->id);
            T_stm s = T_Move(mem, T_Name(Temp_namedlabel(id)));
            T_stm seq = T_Seq(s, NULL);
            head->u.SEQ.right = seq;
            head = head->u.SEQ.right;
        }
    }
    eliminate_seq_null(ret);
    return Tr_Ex(T_Eseq(ret, T_Temp(t)));
}

Tr_exp translate_BoolConst(FILE* out, methodEntry me, A_exp e) {
    if (!e) {
        return NULL;
    }
    if (e->u.b) {
        return Tr_Ex(T_Const(1));
    } else {
        return Tr_Ex(T_Const(0));
    }
}

Tr_exp translate_NumConst(FILE* out, methodEntry me, A_exp e) {
    if (!e) {
        return NULL;
    }
    return Tr_Ex(T_Const(e->u.num));
}

Tr_exp translate_LengthExp(FILE* out,methodEntry me, A_exp e) {
    if (!e) {
        return NULL;
    }
    T_exp exp = unEx(translate_Exp(out, me, e->u.e));
    return Tr_Ex(T_Mem(T_Binop(T_plus, exp, T_Binop(T_mul, T_Const(-1), T_Const(byte_length)))));
}

Tr_exp translate_IdExp(FILE* out, methodEntry me, A_exp e) {
    if (!e) {
        return NULL;
    }
    return Tr_Ex(T_Temp(S_look(me->tempTable, S_Symbol(e->u.v))));
}

Tr_exp translate_NotExp(FILE* out, methodEntry me, A_exp e) {
    if (!e) {
        return NULL;
    }
    Tr_exp exp = translate_Exp(out, me, e->u.e);
    T_stm cond = T_Cjump(T_eq, unEx(exp), T_Const(0), NULL, NULL);
	patchList trues = PatchList(&cond->u.CJUMP.true, NULL);
	patchList falses = PatchList(&cond->u.CJUMP.false, NULL);
	return Tr_Cx(trues, falses, cond);
}

Tr_exp translate_MinusExp(FILE* out, methodEntry me,  A_exp e) {
    if (!e) {
        return NULL;
    }
    Tr_exp exp = translate_Exp(out, me, e->u.e);
    return Tr_Ex(T_Binop(T_minus, T_Const(0), unEx(exp)));
}

Tr_exp translate_EscExp(FILE* out, methodEntry me, A_exp e) {
    if (!e) {
        return NULL;
    }
    Tr_exp stm = translate_StmList(out, me, e->u.escExp.ns);
    Tr_exp exp = translate_Exp(out, me, e->u.escExp.exp);
    if (stm == NULL) {
        return exp;
    }
    return Tr_Ex(T_Eseq(unNx(stm), unEx(exp)));
}

Tr_exp translate_Getint(FILE* out, methodEntry me, A_exp e) {
    if (!e) {
        return NULL;
    }
    return Tr_Ex(T_ExtCall(String("getint"), NULL));
}

Tr_exp translate_Getch(FILE* out, methodEntry me, A_exp e) {
    if (!e) {
        return NULL;
    }
    return Tr_Ex(T_ExtCall(String("getch"), NULL));
}

Tr_exp translate_Getarray(FILE* out, methodEntry me, A_exp e) {
    if (!e) {
        return NULL;
    }
    Tr_exp exp = translate_Exp(out, me, e->u.e);
    return Tr_Ex(T_ExtCall(String("getarray"), T_ExpList(unEx(exp), NULL)));
}

Tr_exp translate_Exp(FILE* out, methodEntry me, A_exp e) {
    if (!e) {
        return NULL;
    }
    switch (e->kind) {
    case A_opExp: return translate_OpExp(out, me, e);
    case A_arrayExp: return translate_ArrayExp(out, me, e);
    case A_callExp: return translate_CallExp(out, me, e);
    case A_classVarExp: return translate_ClassVarExp(out, me, e);
    case A_boolConst: return translate_BoolConst(out, me, e);
    case A_numConst: return translate_NumConst(out, me, e);
    case A_lengthExp: return translate_LengthExp(out, me, e);
    case A_idExp: return translate_IdExp(out, me, e);
    case A_thisExp: return translate_ThisExp(out, me, e);
    case A_newIntArrExp: return translate_NewIntArrExp(out, me, e);
    case A_newObjExp: return translate_NewObjExp(out, me, e);
    case A_notExp: return translate_NotExp(out, me, e);
    case A_minusExp: return translate_MinusExp(out, me, e);
    case A_escExp: return translate_EscExp(out, me, e);
    case A_getint: return translate_Getint(out, me, e);
    case A_getch: return translate_Getch(out, me, e);
    case A_getarray: return translate_Getarray(out, me, e);
    default: fprintf(out, "Unknown expression kind!\n"); fflush(out); exit(1);
    }
    return NULL;
}

Tr_exp translate_NestedStm(FILE* out, methodEntry me, A_stm s) {
    if (!s) {
        return NULL;
    }
    if (s->u.ns == NULL) {
        return Tr_Nx(T_Exp(T_Const(0)));
    }
    return translate_StmList(out, me, s->u.ns);
}

Tr_exp translate_IfStm(FILE* out, methodEntry me, A_stm s) {
    if (!s) {
        return NULL;
    }
    Temp_label t = Temp_newlabel();
    Temp_label f = Temp_newlabel();
    struct Cx condition = unCx(translate_Exp(out, me, s->u.if_stat.e));
    doPatch(condition.trues, t);
	doPatch(condition.falses, f);
    if (!s->u.if_stat.s2) {
        T_stm s1 = unNx(translate_Stm(out, me, s->u.if_stat.s1));
        return  Tr_Nx(T_Seq(condition.stm,
                    T_Seq(T_Label(t),
                        T_Seq(s1,
                            T_Label(f)))));
    } else {
        T_stm s1 = unNx(translate_Stm(out, me, s->u.if_stat.s1));
        T_stm s2 = unNx(translate_Stm(out, me, s->u.if_stat.s2));
        Temp_label e = Temp_newlabel();
        return Tr_Nx(T_Seq(condition.stm, 
                    T_Seq(T_Label(t), 
                        T_Seq(s1,
                            T_Seq(T_Jump(e),
                                T_Seq(T_Label(f),
                                    T_Seq(s2,
                                        T_Label(e)))))))); 
    }
}

Tr_exp translate_WhileStm(FILE* out, methodEntry me, A_stm s) {
    if (!s) {
        return NULL;
    }
    struct Cx condition = unCx(translate_Exp(out, me, s->u.while_stat.e));
    Temp_label t = Temp_newlabel();
    Temp_label f = Temp_newlabel();
    doPatch(condition.trues, t);
    doPatch(condition.falses, f);
    if (!s->u.while_stat.s || (s->u.while_stat.s->kind == A_nestedStm && s->u.while_stat.s->u.ns == NULL)) {
        return Tr_Nx(T_Seq(T_Label(t), 
                    T_Seq(condition.stm, 
                        T_Label(f))));
    } else {
        Temp_label b = Temp_newlabel();
        push_stack(b, f);
        T_stm stm = unNx(translate_Stm(out, me, s->u.while_stat.s));
        pop_stack();
        return Tr_Nx(T_Seq(T_Label(b),
                    T_Seq(condition.stm,
                        T_Seq(T_Label(t),
                            T_Seq(stm,
                                T_Seq(T_Jump(b),
                                    T_Label(f)))))));
    }
}

Tr_exp translate_AssignStm(FILE* out, methodEntry me, A_stm s) {
    if (!s) {
        return NULL;
    }
    Tr_exp arr = translate_Exp(out, me, s->u.assign.arr);
    Tr_exp val = translate_Exp(out, me, s->u.assign.value);
    return Tr_Nx(T_Move(unEx(arr), unEx(val)));
}

Tr_exp translate_ArrayInit(FILE* out, methodEntry me, A_stm s) {
    if (!s) {
        return NULL;
    }
    Tr_exp t = translate_Exp(out, me, s->u.array_init.arr);
    return translate_ArrayExpList(out, me, s->u.array_init.init_values, unEx(t));
}

Tr_exp translate_CallStm(FILE* out, methodEntry me, A_stm s) {
    if (!s) {
        return NULL;
    }
    Ty_ty ty = check_Exp(out, me, s->u.call_stat.obj);
    classEntry ce = (classEntry)S_look(classTable, S_Symbol(ty->id));
    methodEntry localme = (methodEntry)S_look(ce->methodTable, S_Symbol(s->u.call_stat.fun));
    Tr_exp obj = translate_Exp(out, me, s->u.call_stat.obj);
    Temp_temp t = Temp_newtemp();
    return Tr_Nx(
        T_Seq(T_Move(T_Temp(t), unEx(obj)),
        T_Exp(T_Call(s->u.call_stat.fun, 
            T_Binop(T_plus, T_Temp(t), T_Binop(T_mul, T_Const(localme->offset), T_Const(byte_length))),
            T_ExpList(T_Temp(t), transform_ExpList(out, me, s->u.call_stat.el))))));
}

Tr_exp translate_Continue(FILE* out, methodEntry me, A_stm s) {
    if (!s) {
        return NULL;
    }
    if (!loop_stack()) {
        fprintf(out, "line: %d:%d: continue is not in WhileStm!\n", s->pos->line, s->pos->pos);
        fflush(out);
        exit(1);
    }
    return Tr_Nx(T_Jump(cs->jl->begin));
}

Tr_exp translate_Break(FILE* out, methodEntry me, A_stm s) {
    if (!s) {
        return NULL;
    }
    if (!loop_stack()) {
        fprintf(out, "line: %d:%d: break is not in WhileStm!\n", s->pos->line, s->pos->pos);
        fflush(out);
        exit(1);
    }
    return Tr_Nx(T_Jump(cs->jl->end));
}

Tr_exp translate_Return(FILE* out, methodEntry me, A_stm s) {
    if (!s) {
        return NULL;
    }
    T_exp ret = unEx(translate_Exp(out, me, s->u.e));
    return Tr_Nx(T_Return(ret));
}

Tr_exp translate_Putint(FILE* out, methodEntry me, A_stm s) {
    if (!s) {
        return NULL;
    }
    return Tr_Nx(T_Exp(T_ExtCall(String("putint"), T_ExpList(unEx(translate_Exp(out, me, s->u.e)), NULL))));
}

Tr_exp translate_Putch(FILE* out, methodEntry me, A_stm s) {
    if (!s) {
        return NULL;
    }
    return Tr_Nx(T_Exp(T_ExtCall(String("putch"), T_ExpList(unEx(translate_Exp(out, me, s->u.e)), NULL))));
}

Tr_exp translate_Putarray(FILE* out, methodEntry me, A_stm s) {
    if (!s) {
        return NULL;
    }
    Tr_exp e1 = translate_Exp(out, me, s->u.putarray.e1);
    Tr_exp e2 = translate_Exp(out, me, s->u.putarray.e2);
    return Tr_Nx(T_Exp(T_ExtCall(String("putarray"), T_ExpList(unEx(e1), T_ExpList(unEx(e2), NULL)))));
}

Tr_exp translate_Starttime(FILE* out, methodEntry me, A_stm s) {
    if (!s) {
        return NULL;
    }
    return Tr_Nx(T_Exp(T_ExtCall(String("starttime"), NULL)));
}

Tr_exp translate_Stoptime(FILE* out, methodEntry me, A_stm s) {
    if (!s) {
        return NULL;
    }
    return Tr_Nx(T_Exp(T_ExtCall(String("stoptime"), NULL)));
}

Tr_exp translate_Stm(FILE* out, methodEntry me, A_stm s) {
    if (!s) {
        return NULL;
    }
    switch (s->kind) {
    case A_nestedStm: return translate_NestedStm(out, me, s);
    case A_ifStm: return translate_IfStm(out, me, s);
    case A_whileStm: return translate_WhileStm(out, me, s);
    case A_assignStm: return translate_AssignStm(out, me, s);
    case A_arrayInit: return translate_ArrayInit(out, me, s);
    case A_callStm: return translate_CallStm(out, me, s);
    case A_continue: return translate_Continue(out, me, s);
    case A_break: return translate_Break(out, me, s);
    case A_return: return translate_Return(out, me, s);
    case A_putint: return translate_Putint(out, me, s);
    case A_putarray: return translate_Putarray(out, me, s);
    case A_putch: return translate_Putch(out, me, s);
    case A_starttime: return translate_Starttime(out, me, s);
    case A_stoptime: return translate_Stoptime(out, me, s);
    default: fprintf(out, "Unknown statement kind!"); fflush(out); exit(1);
    };
    return NULL;
}

Tr_exp translate_StmList(FILE* out, methodEntry me, A_stmList sl) {
    if (!sl) {
        return NULL;
    }
    Tr_exp left = translate_Stm(out, me, sl->head);
    Tr_exp right = NULL;
    if (sl->tail) {
        right = translate_StmList(out, me, sl->tail);
    }
    if (!left && !right) {
        return NULL;
    } else if (!left) {
        return right;
    } else if (!right) {
        return left;
    } else {
        return Tr_Nx(T_Seq(unNx(left), unNx(right)));
    }
}

Tr_exp translate_VarDecl(FILE* out, methodEntry me, A_varDecl vd) {
    if (!vd) {
        return NULL;
    }
    Temp_temp t = Temp_newtemp();
    S_table tempTable = me->tempTable;
    S_enter(tempTable, S_Symbol(vd->v), t);
    if (vd->elist) {
        if (vd->t->t == A_intType) {
            return Tr_Nx(T_Move(T_Temp(t), T_Const(vd->elist->head->u.num)));
        } else if (vd->t->t == A_intArrType) {
            return translate_ArrayExpList(out, me, vd->elist, T_Temp(t));
        }
    }
    return NULL;
}

Tr_exp translate_VarDeclList(FILE* out, methodEntry me, A_varDeclList vdl) {
    if (!vdl) {
        return NULL;
    }
    Tr_exp left = translate_VarDecl(out, me, vdl->head);
    Tr_exp right = NULL;
    if (vdl->tail) {
        right = translate_VarDeclList(out, me, vdl->tail);
    }
    if (!left && !right) {
        return NULL;
    } else if (!left) {
        return right;
    } else if (!right) {
        return left;
    } else {
        return Tr_Nx(T_Seq(unNx(left), unNx(right)));
    }
}

Temp_temp translate_Formal(FILE* out, methodEntry me, A_formal f) {
    if (!f) {
        return NULL;
    }
    S_table tempTable = me->tempTable;
    Temp_temp t = Temp_newtemp();
    S_enter(tempTable, S_Symbol(f->id), t);
    return t;
}

Temp_tempList translate_FormalList(FILE* out, methodEntry me, A_formalList fl) {
    if (!fl) {
        return NULL;
    }
    Temp_temp h = translate_Formal(out, me, fl->head);
    Temp_tempList t = NULL;
    if (fl->tail) {
        t = translate_FormalList(out, me, fl->tail);
    }
    return Temp_TempList(h, t);
}

T_funcDecl translate_MethodDecl(FILE* out, classEntry ce, A_methodDecl md) {
    if (!md) {
        return NULL;
    }
    Tr_exp left = NULL, right = NULL;
    methodEntry me = (methodEntry)S_look(ce->methodTable, S_Symbol(md->id));
    Temp_tempList tl = NULL;
    // add this
    Temp_temp t = Temp_newtemp();
    me->this = t;
    if (md->fl) {
        tl = translate_FormalList(out, me, md->fl);
    }
    if (md->vdl) {
        left = translate_VarDeclList(out, me, md->vdl);
    }
    if (md->sl) {
        right = translate_StmList(out, me, md->sl);
    }

    T_stm s = NULL;
    if (!left && !right) {
        s = NULL;
    } else if (!left) {
        s = unNx(right);
    } else if (!right) {
        s = unNx(left);
    } else {
        s = T_Seq(unNx(left), unNx(right));
    }

    char* id = (char*)malloc(strlen(me->from->id) + strlen(md->id) + 2);
    sprintf(id, "%s%c%s", me->from->id, '_', md->id);
    return T_FuncDecl(id, Temp_TempList(t, tl), s);
}

T_funcDeclList translate_MethodDeclList(FILE* out, classEntry ce, A_methodDeclList mdl) {
    if (!mdl) {
        return NULL;
    }
    T_funcDecl left = translate_MethodDecl(out, ce, mdl->head);
    T_funcDeclList right = NULL;
    if (mdl->tail) {
        right = translate_MethodDeclList(out, ce, mdl->tail);
    }
    return T_FuncDeclList(left, right);
}

T_funcDecl translate_MainMethod(FILE* out, methodEntry me, A_mainMethod main) {
    Tr_exp left = NULL, right = NULL;
    if (main->vdl) {
        left = translate_VarDeclList(out, me, main->vdl);
    }
    if (main->sl) {
        right = translate_StmList(out, me, main->sl);
    }

    T_stm s = NULL;
    if (!left && !right) {
        s = NULL;
    } else if (!left) {
        s = unNx(right);
    } else if (!right) {
        s = unNx(left);
    } else {
        s = T_Seq(unNx(left), unNx(right));
    }
    return T_FuncDecl(String("main"), NULL, s);
}

T_funcDeclList translate_ClassDecl(FILE* out, A_classDecl cd) {
    return translate_MethodDeclList(out, (classEntry)S_look(classTable, S_Symbol(cd->id)), cd->mdl);
}

T_funcDeclList translate_ClassDeclList(FILE* out, A_classDeclList cdl) {
    T_funcDeclList left = translate_ClassDecl(out, cdl->head);
    T_funcDeclList right = NULL;
    if (cdl->tail) {
        right = translate_ClassDeclList(out, cdl->tail);
    }
    return joinFuncDeclList(left, right);
}

T_funcDeclList translate_Prog(FILE* out, A_prog p) {
    T_funcDecl fd = NULL;
    T_funcDeclList fdl = NULL;
    if (p->m) {
        fd = translate_MainMethod(out, mainMethodEntry, p->m);
    }
    if (p->cdl) {
        fdl = translate_ClassDeclList(out, p->cdl);
    }
    return T_FuncDeclList(fd, fdl);
}