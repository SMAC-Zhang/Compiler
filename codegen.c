#include <string.h>

#include "codegen.h"
#include "temp.h"
#include "util.h"

static AS_instrList iList, last;
static Temp_temp munchExp(T_exp e);
static void munchStm(T_stm s);
static void emit(AS_instr inst);
static void munch_binop(Temp_temp ret, T_exp e);
static Temp_tempList munch_args(int n, T_expList el, string s, bool first);
static void munch_extcall(Temp_temp ret, T_exp e);
static void munch_call(Temp_temp ret, T_exp e);

static void emit(AS_instr inst) {
    if (last != NULL) {
        last = last->tail = AS_InstrList(inst, NULL);
    } else {
        last = iList = AS_InstrList(inst, NULL);
    }
} 

static void munch_binop(Temp_temp ret, T_exp e) {
    char *op = NULL;
    switch (e->u.BINOP.op) {
        case T_plus:  op = "add"; break;
        case T_minus: op = "sub"; break;
        case T_mul:   op = "mul"; break;
        case T_div:   op = "sdiv"; break;
        default: fprintf(stderr, "error in BINOP!"); break;
    }
    if (e->u.BINOP.left->kind == T_CONST && e->u.BINOP.right->kind == T_CONST) {
        int left = e->u.BINOP.left->u.CONST;
        int right = e->u.BINOP.right->u.CONST;
        emit(AS_Oper(String_format("%%`d0 = %s i64 %d, %d", op, left, right),
            Temp_TempList(ret, NULL), NULL, NULL));
    } else if (e->u.BINOP.left->kind == T_CONST) {
        int left = e->u.BINOP.left->u.CONST;
        Temp_temp right = munchExp(e->u.BINOP.right);
        emit(AS_Oper(String_format("%%`d0 = %s i64 %d, %%`s0", op, left),
            Temp_TempList(ret, NULL), Temp_TempList(right, NULL), NULL));
    } else if (e->u.BINOP.right->kind == T_CONST) { 
        Temp_temp left = munchExp(e->u.BINOP.left);
        int right = e->u.BINOP.right->u.CONST;
        emit(AS_Oper(String_format("%%`d0 = %s i64 %%`s0, %d", op, right),
            Temp_TempList(ret, NULL), Temp_TempList(left, NULL), NULL));
    } else {
        Temp_temp left = munchExp(e->u.BINOP.left);
        Temp_temp right = munchExp(e->u.BINOP.right);
        emit(AS_Oper(String_format("%%`d0 = %s i64 %%`s0, %%`s1", op),
            Temp_TempList(ret, NULL), Temp_TempList(left, Temp_TempList(right, NULL)), NULL));
    }
}

//s需要足够大的空间
static Temp_tempList munch_args(int n, T_expList el, string s, bool first) {
	if (!el) {
        return NULL;
    }
	
    Temp_temp e = NULL;
    if (el->head->kind == T_CONST) {
        if (first) {
            strcat(s, String_format("i64 %d", el->head->u.CONST));
        } else {
            strcat(s, String_format(", i64 %d", el->head->u.CONST));
        }
        n--;
    } else {
        e = munchExp(el->head);
        if (first) {
            strcat(s, String_format("i64 %%`s%d", n));
        } else {
            strcat(s, String_format(", i64 %%`s%d", n));
        }
    }

    Temp_tempList tlist = munch_args(n + 1, el->tail, s, FALSE);
    
    if (e == NULL) {
        return tlist;
    } else {
	    return Temp_TempList(e, tlist);
    }
}

// only malloc
static void munch_extcall(Temp_temp ret, T_exp e) {
    Temp_temp r1 = Temp_newtemp();
    char args[1024] = {'\0'};
    Temp_tempList tl = munch_args(0, e->u.ExtCALL.args, args, TRUE);
    emit(AS_Oper(String_format("%%`d0 = call ptr @%s(%s)", e->u.ExtCALL.extfun, String(args)), 
        Temp_TempList(r1, NULL), tl, NULL));
    emit(AS_Oper(String_format("%%`d0 = ptrtoint ptr %%`s0 to i64"),
        Temp_TempList(ret, NULL), Temp_TempList(r1, NULL), NULL));
}

static void munch_call(Temp_temp ret, T_exp e) {
    Temp_temp f1 = munchExp(e->u.CALL.obj);
    Temp_temp f2 = Temp_newtemp();
    Temp_temp f3 = Temp_newtemp();
    Temp_temp f4 = Temp_newtemp();
    emit(AS_Oper(String_format("%%`d0 = inttoptr i64 %%`s0 to ptr"), 
        Temp_TempList(f2, NULL), Temp_TempList(f1, NULL), NULL));
    emit(AS_Oper(String_format("%%`d0 = load i64, ptr %%`s0"),
        Temp_TempList(f3, NULL), Temp_TempList(f2, NULL), NULL));
    emit(AS_Oper(String_format("%%`d0 = inttoptr i64 %%`s0 to ptr"), 
        Temp_TempList(f4, NULL), Temp_TempList(f3, NULL), NULL));
    char args[1024] = {'\0'};
    Temp_tempList tl = munch_args(1, e->u.CALL.args, args, TRUE);
    emit(AS_Oper(String_format("%%`d0 = call i64 %%`s0(%s)", String(args)), 
        Temp_TempList(ret, NULL), Temp_TempList(f4, tl), NULL));
}

static Temp_temp munchExp(T_exp e) {
    switch (e->kind) {
        case T_BINOP: {
            char *op = NULL;
            Temp_temp ret = Temp_newtemp();
            munch_binop(ret, e);
            return ret;
        }
        case T_MEM: {
            Temp_temp l1 = Temp_newtemp(), l2 = Temp_newtemp();
            if (e->u.MEM->kind == T_CONST) {
                emit(AS_Oper(String_format("%%`d0 = inttoptr i64 %d to ptr", e->u.MEM->u.CONST),
                    Temp_TempList(l1, NULL), NULL, NULL));
            } else {
                Temp_temp op = munchExp(e->u.MEM);
                emit(AS_Oper(String_format("%%`d0 = inttoptr i64 %%`s0 to ptr"),
                    Temp_TempList(l1, NULL), Temp_TempList(op, NULL), NULL));
            }

            emit(AS_Oper(String_format("%%`d0 = load i64, ptr %%`s0"),
                Temp_TempList(l2, NULL), Temp_TempList(l1, NULL), NULL));
            return l2;
        }
        case T_TEMP: {
            return e->u.TEMP;
        }
        case T_ESEQ: {
            munchStm(e->u.ESEQ.stm);
			return munchExp(e->u.ESEQ.exp);
        }
        case T_NAME: {
            Temp_temp ret = Temp_newtemp();
			emit(AS_Oper(String_format("%%`d0 = ptrtoint ptr @%s to i64", Temp_labelstring(e->u.NAME)),
                Temp_TempList(ret, NULL), NULL, NULL));
            return ret;
        }
        case T_CONST: {
            Temp_temp ret = Temp_newtemp();
            emit(AS_Oper(String_format("%%`d0 = add i64 %d, 0", e->u.CONST), 
                Temp_TempList(ret, NULL), NULL, NULL));
            return ret;
        }
        case T_CALL: {
            Temp_temp ret = Temp_newtemp();
            munch_call(ret, e);
			return ret;
        }
        case T_ExtCALL: {
            Temp_temp ret = Temp_newtemp();
            munch_extcall(ret, e);
            return ret;
        }
        default: fprintf(stderr, "error in munchExp!"); break;
    }
}

static void munchStm(T_stm s) {
    switch (s->kind) {
        case T_SEQ: {
            munchStm(s->u.SEQ.left);
            munchStm(s->u.SEQ.right);
			break;
        }
        case T_LABEL: {
            emit(AS_Label(String_format("%s:", Temp_labelstring(s->u.LABEL)), s->u.LABEL));
			break;
        }
        case T_JUMP: {
            emit(AS_Oper(String_format("br label %%`j0"),
                NULL, NULL, AS_Targets(Temp_LabelList(s->u.JUMP.jump, NULL))));
            break;
        }
        case T_CJUMP: {
            Temp_temp cond = Temp_newtemp();
            Temp_temp s0 = NULL, s1 = NULL;
            if (s->u.CJUMP.left->kind != T_CONST) {
                s0 = munchExp(s->u.CJUMP.left);
            }
            if (s->u.CJUMP.right->kind != T_CONST) {
                s1 = munchExp(s->u.CJUMP.right);
            }
            char *op = NULL;
            switch (s->u.CJUMP.op) {
                case T_eq: op = "eq";  break;
                case T_ne: op = "ne";  break;
                case T_lt: op = "slt"; break;
                case T_gt: op = "sgt"; break;
                case T_le: op = "sle"; break;
                case T_ge: op = "sge"; break;
                default: fprintf(stderr, "error in CJUMP!"); break;
            }
            if (!s0 && !s1) {
                emit(AS_Oper(String_format("%%`d0 = icmp %s i64 %d, %d", op, s->u.CJUMP.left->u.CONST, s->u.CJUMP.right->u.CONST),
                    Temp_TempList(cond, NULL), NULL, NULL)); 
            } else if (!s0) {
                emit(AS_Oper(String_format("%%`d0 = icmp %s i64 %d, %%`s0", op, s->u.CJUMP.left->u.CONST),
                    Temp_TempList(cond, NULL), Temp_TempList(s1, NULL), NULL)); 
            } else if (!s1) {
                emit(AS_Oper(String_format("%%`d0 = icmp %s i64 %%`s0, %d", op, s->u.CJUMP.right->u.CONST),
                    Temp_TempList(cond, NULL), Temp_TempList(s0, NULL), NULL)); 
            } else {
                emit(AS_Oper(String_format("%%`d0 = icmp %s i64 %%`s0, %%`s1", op),
                    Temp_TempList(cond, NULL), Temp_TempList(s0, Temp_TempList(s1, NULL)), NULL)); 
            }

            emit(AS_Oper(String_format("br i1 %%`s0, label %%`j0, label %%`j1"), 
                NULL, Temp_TempList(cond, NULL), AS_Targets(Temp_LabelList(s->u.CJUMP.true, Temp_LabelList(s->u.CJUMP.false, NULL)))));
            break;
        }
        case T_MOVE: {
            if (s->u.MOVE.dst->kind == T_MEM) {
                Temp_temp src = NULL, dst = NULL;
                if (s->u.MOVE.src->kind != T_CONST) {
                    src = munchExp(s->u.MOVE.src);
                }
                if (s->u.MOVE.dst->u.MEM->kind != T_CONST) {
                    dst = munchExp(s->u.MOVE.dst->u.MEM);
                }
                Temp_temp l1 = Temp_newtemp();
                if (s->u.MOVE.dst->u.MEM->kind == T_CONST) {
                    emit(AS_Oper(String_format("%%`d0 = inttoptr i64 %d to ptr", s->u.MOVE.dst->u.MEM->u.CONST),
                        Temp_TempList(l1, 0), NULL, NULL));
                } else {
                    emit(AS_Oper(String_format("%%`d0 = inttoptr i64 %%`s0 to ptr"),
                        Temp_TempList(l1, 0), Temp_TempList(dst, NULL), NULL));
                }
                if (s->u.MOVE.src->kind == T_CONST) {
                    emit(AS_Oper(String_format("store i64 %d, ptr %%`s0", s->u.MOVE.src->u.CONST),
                        NULL, Temp_TempList(l1, NULL), NULL));
                } else {
                    emit(AS_Oper(String_format("store i64 %%`s0, ptr %%`s1"),
                        NULL, Temp_TempList(src, Temp_TempList(l1, NULL)), NULL));
                }
            } else if (s->u.MOVE.src->kind == T_MEM) {
                Temp_temp src = NULL;
                Temp_temp dst = munchExp(s->u.MOVE.dst);
                Temp_temp l1 = Temp_newtemp();
                if (s->u.MOVE.src->u.MEM->kind == T_CONST) {
                    emit(AS_Oper(String_format("%%`d0 = inttoptr i64 %d to ptr", s->u.MOVE.src->u.MEM->u.CONST),
                        Temp_TempList(l1, 0), NULL, NULL));
                } else {
                    src = munchExp(s->u.MOVE.src->u.MEM);
                    emit(AS_Oper(String_format("%%`d0 = inttoptr i64 %%`s0 to ptr"),
                        Temp_TempList(l1, 0), Temp_TempList(src, NULL), NULL));
                }
                emit(AS_Oper(String_format("%%`d0 = load i64, ptr %%`s0"),
                    Temp_TempList(dst, NULL), Temp_TempList(l1, NULL), NULL));
            } else if (s->u.MOVE.dst->kind == T_TEMP && s->u.MOVE.src->kind == T_TEMP){
                Temp_temp src = munchExp(s->u.MOVE.src);
                Temp_temp dst = munchExp(s->u.MOVE.dst);
                emit(AS_Move(String_format("%%`d0 = add i64 %%`s0, 0"),
                    Temp_TempList(dst, NULL), Temp_TempList(src, NULL)));
            } else {
                switch (s->u.MOVE.src->kind) {
                    case T_BINOP: {
                        Temp_temp dst = munchExp(s->u.MOVE.dst);
                        munch_binop(dst, s->u.MOVE.src);
                        break;
                    }
                    case T_CONST: {
                        Temp_temp dst = munchExp(s->u.MOVE.dst);
                        emit(AS_Oper(String_format("%%`d0 = add i64 %d, 0", s->u.MOVE.src->u.CONST),
                            Temp_TempList(dst, NULL), NULL, NULL));
                        break;
                    }
                    case T_ExtCALL: {
                        Temp_temp dst = munchExp(s->u.MOVE.dst);
                        munch_extcall(dst, s->u.MOVE.src);
                        break;
                    }
                    case T_CALL: {
                        Temp_temp dst = munchExp(s->u.MOVE.dst);
                        munch_call(dst, s->u.MOVE.src);
                        break;
                    }
                    default: {
                        Temp_temp src = munchExp(s->u.MOVE.src);
                        Temp_temp dst = munchExp(s->u.MOVE.dst);
                        emit(AS_Oper(String_format("%%`d0 = add i64 %%`s0, 0"),
                            Temp_TempList(dst, NULL), Temp_TempList(src, NULL), NULL));
                        break;
                    }
                }
            }
            break;
        }
        case T_EXP: {
            munchExp(s->u.EXP);
			break;
        }
        case T_RETURN: {
            if (s->u.EXP->kind == T_CONST) {
                emit(AS_Oper(String_format("ret i64 %d", s->u.EXP->u.CONST),
                    NULL, NULL, NULL));
            } else {
                Temp_temp ret = munchExp(s->u.EXP);
                emit(AS_Oper(String_format("ret i64 %%`s0"),
                    NULL, Temp_TempList(ret, NULL), NULL));
            }
            break;
        }
        default: fprintf(stderr, "error in munchStm!"); break;
    }
}

AS_instrList codegen(T_stmList stmList) {
    AS_instrList list = NULL;
    T_stmList sl;
    for (sl = stmList; sl; sl = sl->tail) {
		munchStm(sl->head);
    }
	list = iList;
	iList = last = NULL;
	return list;
}

AS_instrList progen(T_funcDecl fl) {
    emit(AS_Oper(String_format("define i64 @%s(%s) {\n", fl->name, args_to_string(fl->args)), NULL, NULL, NULL));
    AS_instrList list;
    list = iList;
	iList = last = NULL;
	return list;
}

AS_instrList epigen(Temp_label l) {
    emit(AS_Label(String_format("%s:", Temp_labelstring(l)), l));
    emit(AS_Oper(String("ret i64 -1"), NULL, NULL, NULL));
    emit(AS_Oper(String("}"), NULL, NULL, NULL));

    AS_instrList list;
    list = iList;
	iList = last = NULL;
	return list;
} 