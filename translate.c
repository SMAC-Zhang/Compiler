#include <stdlib.h>
#include "translate.h"
#include "temp.h"
#include "symbol.h"

static S_table tempTable;

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
    while (first->tail) {
        first = first->tail;
    }
    first->tail = second;
    return first;
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
        cx.stm = T_Cjump(T_eq, e->u.ex, T_Const(0), NULL, NULL);
        cx.trues = PatchList(&(cx.stm->u.CJUMP.false), NULL);
        cx.falses = PatchList(&(cx.stm->u.CJUMP.true), NULL);
        return cx;
    }
    case Tr_nx: {
        fprintf(stderr, "error in UnCx\n");
        exit(1);
    }
    case Tr_cx: return e->u.cx;
	}

}

Tr_exp translate_OpExp(FILE* out, A_exp e, Temp_label begin, Temp_label end, bool loop) {
    Tr_exp left = translate_Exp(out, e->u.op.left, begin, end, loop);
    Tr_exp right = translate_Exp(out, e->u.op.right, begin, end, loop);
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
        trues = PatchList(&cond->u.SEQ.right->u.SEQ.right->u.CJUMP.true, NULL);
        falses = PatchList(&cond->u.SEQ.left->u.CJUMP.false, 
                    PatchList(&cond->u.SEQ.right->u.SEQ.right->u.CJUMP.false, NULL));
    } else if (e->u.op.oper == A_or) {
        Temp_label f = Temp_newlabel();
        doPatch(leftC.falses, f);
        cond = T_Seq(leftC.stm, T_Seq(T_Label(f), rightC.stm));
        trues = PatchList(&cond->u.SEQ.left->u.CJUMP.true, 
                    PatchList(&cond->u.SEQ.right->u.SEQ.right->u.CJUMP.true, NULL));
        falses = PatchList(&cond->u.SEQ.right->u.SEQ.right->u.CJUMP.false, NULL);
    } else {
        fprintf(out, "UNKNOWN OP kind\n");
        exit(1);
    }
    return Tr_Cx(trues, falses, cond);
}

Tr_exp translate_BoolConst(FILE* out, A_exp e) {
    if (!e) {
        return NULL;
    }
    if (e->u.b) {
        return Tr_Ex(T_Const(1));
    } else {
        return Tr_Ex(T_Const(0));
    }
}

Tr_exp translate_NumConst(FILE* out, A_exp e) {
    if (!e) {
        return NULL;
    }
    return Tr_Ex(T_Const(e->u.num));
}

Tr_exp translate_LengthExp(FILE* out, A_exp e, Temp_label begin, Temp_label end, bool loop) {
    if (!e) {
        return NULL;
    }
    Tr_exp exp = translate_Exp(out, e->u.e, begin, end, loop);
    return Tr_Ex(T_ExtCall(String("length"), T_ExpList(unEx(exp), NULL)));
}

Tr_exp translate_IdExp(FILE* out, A_exp e) {
    if (!e) {
        return NULL;
    }
    return Tr_Ex(T_Temp(S_look(tempTable, S_Symbol(e->u.v))));
}

Tr_exp translate_NotExp(FILE* out, A_exp e, Temp_label begin, Temp_label end, bool loop) {
    if (!e) {
        return NULL;
    }
    Tr_exp exp = translate_Exp(out, e->u.e, begin, end, loop);
    T_stm cond = T_Cjump(T_eq, unEx(exp), T_Const(0), NULL, NULL);
	patchList trues = PatchList(&cond->u.CJUMP.true, NULL);
	patchList falses = PatchList(&cond->u.CJUMP.false, NULL);
	return Tr_Cx(trues, falses, cond);
}

Tr_exp translate_MinusExp(FILE* out, A_exp e, Temp_label begin, Temp_label end, bool loop) {
    if (!e) {
        return NULL;
    }
    T_exp t = T_Temp(Temp_newtemp());
    Tr_exp exp = translate_Exp(out, e->u.e, begin, end, loop);
    T_exp minus = T_Binop(T_minus, T_Const(0), unEx(exp));
    return Tr_Ex(T_Eseq(T_Move(t, minus), t));
}

Tr_exp translate_EscExp(FILE* out, A_exp e, Temp_label begin, Temp_label end, bool loop) {
    if (!e) {
        return NULL;
    }
    Tr_exp stm = translate_StmList(out, e->u.escExp.ns, begin, end, loop);
    Tr_exp exp = translate_Exp(out, e->u.escExp.exp, begin, end, loop);
    if (stm == NULL) {
        return exp;
    }
    return Tr_Ex(T_Eseq(unNx(stm), unEx(exp)));
}

Tr_exp translate_Getint(FILE* out, A_exp e) {
    if (!e) {
        return NULL;
    }
    return Tr_Ex(T_ExtCall(String("getint"), NULL));
}

Tr_exp translate_Getch(FILE* out, A_exp e) {
    if (!e) {
        return NULL;
    }
    return Tr_Ex(T_ExtCall(String("getch"), NULL));
}

Tr_exp translate_Exp(FILE* out, A_exp e, Temp_label begin, Temp_label end, bool loop) {
    if (!e) {
        return NULL;
    }
    switch (e->kind) {
    case A_opExp: return translate_OpExp(out, e, begin, end, loop);
    case A_arrayExp: break;
    case A_callExp: break;
    case A_classVarExp: break;
    case A_boolConst: return translate_BoolConst(out, e);
    case A_numConst: return translate_NumConst(out, e);
    case A_lengthExp: return translate_LengthExp(out, e, begin, end, loop);
    case A_idExp: return translate_IdExp(out, e);
    case A_thisExp: break;
    case A_newIntArrExp: break;
    case A_newObjExp: break;
    case A_notExp: return translate_NotExp(out, e, begin, end, loop);
    case A_minusExp: return translate_MinusExp(out, e, begin, end, loop);
    case A_escExp: return translate_EscExp(out, e, begin, end, loop);
    case A_getint: return translate_Getint(out, e);
    case A_getch: return translate_Getch(out, e);
    case A_getarray: break;
    default: fprintf(out, "Unknown expression kind!\n"); fflush(out); exit(1);
    }
    return NULL;
}

Tr_exp translate_NestedStm(FILE* out, A_stm s, Temp_label begin, Temp_label end, bool loop) {
    if (!s) {
        return NULL;
    }
    return translate_StmList(out, s->u.ns, begin, end, loop);
}

Tr_exp translate_IfStm(FILE* out, A_stm s, Temp_label begin, Temp_label end, bool loop) {
    if (!s) {
        return NULL;
    }
    Temp_label t = Temp_newlabel();
    Temp_label f = Temp_newlabel();
    struct Cx condition = unCx(translate_Exp(out, s->u.if_stat.e, begin, end, loop));
    doPatch(condition.trues, t);
	doPatch(condition.falses, f);
    if (s->u.if_stat.s2) {
        T_stm s1 = unNx(translate_Stm(out, s->u.if_stat.s1, begin, end, loop));
        T_stm s2 = unNx(translate_Stm(out, s->u.if_stat.s2, begin, end, loop));
        Temp_label e = Temp_newlabel();
        return Tr_Nx(T_Seq(condition.stm, 
                    T_Seq(T_Label(t), 
                        T_Seq(s1,
                            T_Seq(T_Jump(e),
                                T_Seq(T_Label(f),
                                    T_Seq(s2,
                                        T_Label(e)))))))); 
    } else {
        T_stm s1 = unNx(translate_Stm(out, s->u.if_stat.s1, begin, end, loop));
        return  Tr_Nx(T_Seq(condition.stm,
                    T_Seq(T_Label(t),
                        T_Seq(s1,
                            T_Label(f)))));
    }
}

Tr_exp translate_WhileStm(FILE* out, A_stm s, Temp_label begin, Temp_label end, bool loop) {
    if (!s) {
        return NULL;
    }
    struct Cx condition = unCx(translate_Exp(out, s->u.while_stat.e, begin, end, loop));
    Temp_label t = Temp_newlabel();
    Temp_label f = Temp_newlabel();
    doPatch(condition.trues, t);
    doPatch(condition.falses, f);
    if (s->u.while_stat.s) {
        Temp_label b = Temp_newlabel();
        T_stm stm = unNx(translate_Stm(out, s->u.while_stat.s, b, f, TRUE));
        return Tr_Nx(T_Seq(T_Label(b),
                    T_Seq(condition.stm,
                        T_Seq(T_Label(t),
                            T_Seq(stm,
                                T_Seq(T_Jump(b),
                                    T_Label(f)))))));
    } else {
        return Tr_Nx(T_Seq(T_Label(t), 
                    T_Seq(condition.stm, 
                        T_Label(f))));
    }
}

Tr_exp translate_AssignStm(FILE* out, A_stm s, Temp_label begin, Temp_label end, bool loop) {
    if (!s) {
        return NULL;
    }
    Tr_exp arr = translate_Exp(out, s->u.assign.arr, begin, end, loop);
    Tr_exp val = translate_Exp(out, s->u.assign.value, begin, end, loop);
    return Tr_Nx(T_Move(unEx(arr), unEx(val)));
}

Tr_exp translate_Continue(FILE* out, A_stm s, Temp_label begin, bool loop) {
    if (!s) {
        return NULL;
    }
    if (!loop) {
        fprintf(out, "line: %d:%d: continue is not in WhileStm!\n", s->pos->line, s->pos->pos);
        fflush(out);
        exit(1);
    }
    return Tr_Nx(T_Jump(begin));
}

Tr_exp translate_Break(FILE* out, A_stm s, Temp_label end, bool loop) {
    if (!s) {
        return NULL;
    }
    if (!loop) {
        fprintf(out, "line: %d:%d: break is not in WhileStm!\n", s->pos->line, s->pos->pos);
        fflush(out);
        exit(1);
    }
    return Tr_Nx(T_Jump(end));
}

Tr_exp translate_Return(FILE* out, A_stm s, Temp_label begin, Temp_label end, bool loop) {
    if (!s) {
        return NULL;
    }
    T_exp ret = unEx(translate_Exp(out, s->u.e, begin, end, loop));
    return Tr_Nx(T_Return(ret));
}

Tr_exp translate_Putint(FILE* out, A_stm s, Temp_label begin, Temp_label end, bool loop) {
    if (!s) {
        return NULL;
    }
    return Tr_Nx(T_Exp(T_ExtCall(String("putint"), T_ExpList(unEx(translate_Exp(out, s->u.e, begin, end, loop)), NULL))));
}

Tr_exp translate_Putch(FILE* out, A_stm s, Temp_label begin, Temp_label end, bool loop) {
    if (!s) {
        return NULL;
    }
    return Tr_Nx(T_Exp(T_ExtCall(String("putch"), T_ExpList(unEx(translate_Exp(out, s->u.e, begin, end, loop)), NULL))));
}

Tr_exp translate_Starttime(FILE* out, A_stm s) {
    if (!s) {
        return NULL;
    }
    return Tr_Nx(T_Exp(T_ExtCall(String("starttime"), NULL)));
}

Tr_exp translate_Stoptime(FILE* out, A_stm s) {
    if (!s) {
        return NULL;
    }
    return Tr_Nx(T_Exp(T_ExtCall(String("stoptime"), NULL)));
}

Tr_exp translate_Stm(FILE* out, A_stm s, Temp_label begin, Temp_label end, bool loop) {
    if (!s) {
        return NULL;
    }
    switch (s->kind) {
    case A_nestedStm: return translate_NestedStm(out, s, begin, end, loop);
    case A_ifStm: return translate_IfStm(out, s, begin, end, loop);
    case A_whileStm: return translate_WhileStm(out, s, begin, end, loop);
    case A_assignStm: return translate_AssignStm(out, s, begin, end, loop);
    case A_arrayInit: break;
    case A_callStm: break;
    case A_continue: return translate_Continue(out, s, begin, loop);
    case A_break: return translate_Break(out, s, end, loop);
    case A_return: return translate_Return(out, s, begin, end, loop);
    case A_putint: return translate_Putint(out, s, begin, end, loop);
    case A_putarray: break;
    case A_putch: return translate_Putch(out, s, begin, end, loop);
    case A_starttime: return translate_Starttime(out, s);
    case A_stoptime: return translate_Stoptime(out, s);
    default: fprintf(out, "Unknown statement kind!"); fflush(out); exit(1);
    };
    return NULL;
}

Tr_exp translate_StmList(FILE* out, A_stmList sl, Temp_label begin, Temp_label end, bool loop) {
    if (!sl) {
        return NULL;
    }
    Tr_exp left = translate_Stm(out, sl->head, begin, end, loop);
    Tr_exp right = NULL;
    if (sl->tail) {
        right = translate_StmList(out, sl->tail, begin, end, loop);
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

Tr_exp translate_VarDecl(FILE* out, A_varDecl vd) {
    if (!vd) {
        return NULL;
    }
    Temp_temp t = Temp_newtemp();
    S_enter(tempTable, S_Symbol(vd->v), t);
    if (vd->elist) {
        return Tr_Nx(T_Move(T_Temp(t), T_Const(vd->elist->head->u.num)));
    }
    return NULL;
}

Tr_exp translate_VarDeclList(FILE* out, A_varDeclList vdl) {
    if (!vdl) {
        return NULL;
    }
    Tr_exp left = translate_VarDecl(out, vdl->head);
    Tr_exp right = NULL;
    if (vdl->tail) {
        right = translate_VarDeclList(out, vdl->tail);
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

T_funcDecl translate_MainMethod(FILE* out, A_mainMethod main) {
    Tr_exp left = NULL, right = NULL;
    if (main->vdl) {
        left = translate_VarDeclList(out, main->vdl);
    }
    if (main->sl) {
        right = translate_StmList(out, main->sl, NULL, NULL, FALSE);
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

T_funcDeclList translate_Prog(FILE* out, A_prog p) {
    tempTable = S_empty();
    T_funcDecl fd = NULL;
    if (p->m) {
        fd = translate_MainMethod(out, p->m);
    }
    return T_FuncDeclList(fd, NULL);
}