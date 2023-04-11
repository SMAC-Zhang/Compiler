#include "translate.h"
#include "temp.h"
#include "symbol.h"

static S_table tempTable;

T_exp translate_OpExp(FILE* out, A_exp e, Temp_label begin, Temp_label end, bool loop) {
    T_exp left = translate_Exp(out, e->u.op.left, begin, end, loop);
    T_exp right = translate_Exp(out, e->u.op.right, begin, end, loop);
    Temp_label t = NULL;
    Temp_label f = NULL;
    T_exp temp = NULL;
    if (e->u.op.oper != A_plus && e->u.op.oper != A_minus 
    && e->u.op.oper != A_times && e->u.op.oper != A_div) {
        t = Temp_newlabel();
        f = Temp_newlabel();
        temp = T_Temp(Temp_newtemp());    
    }
    // 短路
    if (e->u.op.oper == A_and) {
        return T_Eseq(T_Move(temp, T_Const(0)), 
                T_Eseq(T_Cjump(T_eq, T_Const(0), left, t, f), 
                    T_Eseq(T_Label(f), 
                        T_Eseq(T_Move(temp, right), 
                            T_Eseq(T_Label(t), temp)))));
    } else if (e->u.op.oper == A_or) {
        return T_Eseq(T_Move(temp, T_Const(1)), 
                    T_Eseq(T_Cjump(T_ne, T_Const(0), left, t, f), 
                        T_Eseq(T_Label(f),
                            T_Eseq(T_Move(temp, right),
                                T_Eseq(T_Label(t), temp)))));
    }
    // 普通二元运算符
    switch (e->u.op.oper) {
    case A_less: {
        return T_Eseq(T_Move(temp, T_Const(1)), 
            T_Eseq(T_Cjump(T_lt, left, right, t, f), 
                T_Eseq(T_Label(f), 
                    T_Eseq(T_Move(temp, T_Const(0)), 
                        T_Eseq(T_Label(t), temp)))));
    }
    case A_le: {
        return T_Eseq(T_Move(temp, T_Const(1)), 
            T_Eseq(T_Cjump(T_le, left, right, t, f), 
                T_Eseq(T_Label(f), 
                    T_Eseq(T_Move(temp, T_Const(0)), 
                        T_Eseq(T_Label(t), temp)))));
    }
    case A_greater: {
        return T_Eseq(T_Move(temp, T_Const(1)), 
            T_Eseq(T_Cjump(T_gt, left, right, t, f), 
                T_Eseq(T_Label(f), 
                    T_Eseq(T_Move(temp, T_Const(0)), 
                        T_Eseq(T_Label(t), temp)))));
    }
    case A_ge: {
        return T_Eseq(T_Move(temp, T_Const(1)), 
            T_Eseq(T_Cjump(T_ge, left, right, t, f), 
                T_Eseq(T_Label(f), 
                    T_Eseq(T_Move(temp, T_Const(0)), 
                        T_Eseq(T_Label(t), temp)))));
    }
    case A_eq: {
        return T_Eseq(T_Move(temp, T_Const(1)), 
            T_Eseq(T_Cjump(T_eq, left, right, t, f), 
                T_Eseq(T_Label(f), 
                    T_Eseq(T_Move(temp, T_Const(0)), 
                        T_Eseq(T_Label(t), temp)))));
    }
    case A_ne:  {
        return T_Eseq(T_Move(temp, T_Const(1)), 
            T_Eseq(T_Cjump(T_ne, left, right, t, f), 
                T_Eseq(T_Label(f), 
                    T_Eseq(T_Move(temp, T_Const(0)), 
                        T_Eseq(T_Label(t), temp)))));
    }
    case A_plus: return T_Binop(T_plus, left, right);
    case A_minus: return T_Binop(T_minus, left, right);
    case A_times: return T_Binop(T_mul, left, right);
    case A_div: return T_Binop(T_div, left, right);
    default: fprintf(out, "UNKNOWN kind OpExp\n"); fflush(out); exit(1);
    }
    return NULL;
}

T_exp translate_BoolConst(FILE* out, A_exp e) {
    if (!e) {
        return NULL;
    }
    if (e->u.b) {
        return T_Const(1);
    } else {
        return T_Const(0);
    }
}

T_exp translate_NumConst(FILE* out, A_exp e) {
    if (!e) {
        return NULL;
    }
    return T_Const(e->u.num);
}

T_exp translate_LengthExp(FILE* out, A_exp e) {
    if (!e) {
        return NULL;
    }
    return T_ExtCall(String("length"), T_ExpList(T_Temp(S_look(tempTable, S_Symbol(e->u.v))), NULL));
}

T_exp translate_IdExp(FILE* out, A_exp e) {
    if (!e) {
        return NULL;
    }
    return T_Temp(S_look(tempTable, S_Symbol(e->u.v)));
}

T_exp translate_NotExp(FILE* out, A_exp e, Temp_label begin, Temp_label end, bool loop) {
    if (!e) {
        return NULL;
    }
    T_exp temp = T_Temp(Temp_newtemp());
    T_exp exp = translate_Exp(out, e->u.e, begin, end, loop);
    Temp_label t = Temp_newlabel(), f = Temp_newlabel();
    return T_Eseq(T_Move(temp, T_Const(0)), 
                T_Eseq(T_Cjump(T_ne, T_Const(0), exp, t, f),
                    T_Eseq(T_Label(f), 
                        T_Eseq(T_Move(temp, T_Const(1)), 
                            T_Eseq(T_Label(t), temp)))));
}

T_exp translate_MinusExp(FILE* out, A_exp e, Temp_label begin, Temp_label end, bool loop) {
    if (!e) {
        return NULL;
    }
    T_exp t = T_Temp(Temp_newtemp());
    T_exp exp = translate_Exp(out, e->u.e, begin, end, loop);
    T_exp minus = T_Binop(T_minus, 0, exp);
    return T_Eseq(T_Move(t, minus), t);
}

T_exp translate_EscExp(FILE* out, A_exp e, Temp_label begin, Temp_label end, bool loop) {
    if (!e) {
        return NULL;
    }
    T_stm stm = translate_StmList(out, e->u.escExp.ns, begin, end, loop);
    T_exp exp = translate_Exp(out, e->u.escExp.exp, begin, end, loop);
    return T_Eseq(stm, exp);
}

T_exp translate_Getint(FILE* out, A_exp e) {
    if (!e) {
        return NULL;
    }
    return T_ExtCall(String("getint"), NULL);
}

T_exp translate_Getch(FILE* out, A_exp e) {
    if (!e) {
        return NULL;
    }
    return T_ExtCall(String("getch"), NULL);
}

T_exp translate_Exp(FILE* out, A_exp e, Temp_label begin, Temp_label end, bool loop) {
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
    case A_lengthExp: return translate_LengthExp(out, e);
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

T_stm translate_NestedStm(FILE* out, A_stm s, Temp_label begin, Temp_label end, bool loop) {
    if (!s) {
        return NULL;
    }
    return translate_StmList(out, s->u.ns, begin, end, loop);
}

T_stm translate_IfStm(FILE* out, A_stm s, Temp_label begin, Temp_label end, bool loop) {
    if (!s) {
        return NULL;
    }
    Temp_label trues = Temp_newlabel();
    Temp_label falses = Temp_newlabel();
    T_exp exp = translate_Exp(out, s->u.if_stat.e, begin, end, loop);
    if (s->u.if_stat.s2) {
        Temp_label stops = Temp_newlabel();
        return  T_Seq(T_Cjump(T_ne, T_Const(0), exp, trues, falses), 
                    T_Seq(T_Label(trues), 
                        T_Seq(translate_Stm(out, s->u.if_stat.s1, begin, end, loop), 
                            T_Seq(T_Jump(stops), 
                                T_Seq(T_Label(falses), 
                                    T_Seq(translate_Stm(out, s->u.if_stat.s2, begin, end, loop),
                                        T_Label(stops)))))));
    } else {
        return  T_Seq(T_Cjump(T_ne, T_Const(0), exp, trues, falses), 
                    T_Seq(T_Label(trues), 
                        T_Seq(translate_Stm(out, s->u.if_stat.s1, begin, end, loop), 
                            T_Label(falses))));
    }
}

T_stm translate_WhileStm(FILE* out, A_stm s, Temp_label begin, Temp_label end, bool loop) {
    if (!s) {
        return NULL;
    }
    T_exp exp =  translate_Exp(out, s->u.if_stat.e, begin, end, loop);
    if (s->u.while_stat.s) {
        Temp_label starts = Temp_newlabel();
        Temp_label trues = Temp_newlabel();
        Temp_label stops = Temp_newlabel();
        return T_Seq(T_Label(starts), 
                    T_Seq(T_Cjump(T_ne, T_Const(0), exp, trues, stops),
                        T_Seq(T_Label(trues), 
                            T_Seq(translate_Stm(out, s->u.while_stat.s, starts, stops, TRUE), 
                                T_Seq(T_Jump(starts),
                                    T_Label(stops))))));
    } else {
        Temp_label starts = Temp_newlabel();
        Temp_label stops = Temp_newlabel();
        return T_Seq(T_Label(starts), 
                    T_Seq(T_Cjump(T_ne, T_Const(0), exp, starts, stops), 
                        T_Label(stops)));
    }
}

T_stm translate_AssignStm(FILE* out, A_stm s, Temp_label begin, Temp_label end, bool loop) {
    if (!s) {
        return NULL;
    }
    T_exp arr = translate_Exp(out, s->u.assign.arr, begin, end, loop);
    T_exp val = translate_Exp(out, s->u.assign.value, begin, end, loop);
    return T_Move(arr, val);
}

T_stm translate_Continue(FILE* out, A_stm s, Temp_label begin, bool loop) {
    if (!s) {
        return NULL;
    }
    if (!loop) {
        fprintf(out, "line: %d:%d: continue is not in WhileStm!\n", s->pos->line, s->pos->pos);
        fflush(out);
        exit(1);
    }
    return T_Jump(begin);
}

T_stm translate_Break(FILE* out, A_stm s, Temp_label end, bool loop) {
    if (!s) {
        return NULL;
    }
    if (!loop) {
        fprintf(out, "line: %d:%d: break is not in WhileStm!\n", s->pos->line, s->pos->pos);
        fflush(out);
        exit(1);
    }
    return T_Jump(end);
}

T_stm translate_Return(FILE* out, A_stm s, Temp_label begin, Temp_label end, bool loop) {
    if (!s) {
        return NULL;
    }
    T_exp ret = translate_Exp(out, s->u.e, begin, end, loop);
    return T_Return(ret);
}

T_stm translate_Putint(FILE* out, A_stm s, Temp_label begin, Temp_label end, bool loop) {
    if (!s) {
        return NULL;
    }
    return T_Exp(T_ExtCall(String("putint"), T_ExpList(translate_Exp(out, s->u.e, begin, end, loop), NULL)));
}

T_stm translate_Putch(FILE* out, A_stm s, Temp_label begin, Temp_label end, bool loop) {
    if (!s) {
        return NULL;
    }
    return T_Exp(T_ExtCall(String("putch"), T_ExpList(translate_Exp(out, s->u.e, begin, end, loop), NULL)));
}

T_stm translate_Starttime(FILE* out, A_stm s) {
    if (!s) {
        return NULL;
    }
    return T_Exp(T_ExtCall(String("starttime"), NULL));
}

T_stm translate_Stoptime(FILE* out, A_stm s) {
    if (!s) {
        return NULL;
    }
    return T_Exp(T_ExtCall(String("stoptime"), NULL));
}

T_stm translate_Stm(FILE* out, A_stm s, Temp_label begin, Temp_label end, bool loop) {
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

T_stm translate_StmList(FILE* out, A_stmList sl, Temp_label begin, Temp_label end, bool loop) {
    if (!sl) {
        return NULL;
    }
    T_stm left = translate_Stm(out, sl->head, begin, end, loop);
    T_stm right = NULL;
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
        return T_Seq(left, right);
    }
}

T_stm translate_VarDecl(FILE* out, A_varDecl vd) {
    if (!vd) {
        return NULL;
    }
    Temp_temp t = Temp_newtemp();
    S_enter(tempTable, S_Symbol(vd->v), t);
    if (vd->elist) {
        return T_Move(T_Temp(t), T_Const(vd->elist->head->u.num));
    }
    return NULL;
}

T_stm translate_VarDeclList(FILE* out, A_varDeclList vdl) {
    if (!vdl) {
        return NULL;
    }
    T_stm left = translate_VarDecl(out, vdl->head);
    T_stm right = NULL;
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
        return T_Seq(left, right);
    }
}

T_funcDecl translate_MainMethod(FILE* out, A_mainMethod main) {
    T_stm left = NULL, right = NULL;
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
        s = right;
    } else if (!right) {
        s = left;
    } else {
        s = T_Seq(left, right);
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