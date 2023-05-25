#include "irp2rpi.h"

static AS_instrList iList, last;
static Temp_temp ret_addr;
static AS_instrList codegen(T_stmList stmList);
static AS_instrList progen(T_funcDecl fl);
static AS_instrList epigen(Temp_label tl);
static Temp_temp munchExp(T_exp e);
static void munchStm(T_stm s);
static void emit(AS_instr inst);

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
    Temp_temp left = munchExp(e->u.BINOP.left);
    Temp_temp right = munchExp(e->u.BINOP.right);
    if (e->u.BINOP.op == T_plus || e->u.BINOP.op == T_minus) {
        emit(AS_Oper(String_format("%s `d0, `s0, `s1", op),
            Temp_TempList(ret, NULL), Temp_TempList(left, Temp_TempList(right, NULL)), NULL));
    } else {
        if (left == ret && right == ret) {
            Temp_temp origin_left = left, origin_right = right;
            left = Temp_newtemp(); 
            right = Temp_newtemp();
            emit(AS_Oper(String_format("add `d0, `s0, 0"),
                Temp_TempList(left, NULL), Temp_TempList(origin_left, NULL), NULL));
            emit(AS_Oper(String_format("add `d0, `s0, 0"),
                Temp_TempList(right, NULL), Temp_TempList(origin_right, NULL), NULL));            
        } else if (left == ret) {
            Temp_temp origin_left = left;
            left = Temp_newtemp(); 
            emit(AS_Oper(String_format("add `d0, `s0, 0"),
                Temp_TempList(left, NULL), Temp_TempList(origin_left, NULL), NULL));
        } else if (right == ret) {
            Temp_temp origin_right = right;
            right = Temp_newtemp(); 
            emit(AS_Oper(String_format("add `d0, `s0, 0"),
                Temp_TempList(right, NULL), Temp_TempList(origin_right, NULL), NULL));
        }
        emit(AS_Oper(String_format("%s `d0, `s0, `s1", op),
            Temp_TempList(ret, NULL), Temp_TempList(left, Temp_TempList(right, NULL)), NULL));
    }
}

static void munch_args(T_expList el) {
    Temp_tempList tl = NULL;
    int i = 0;
    while (el) {
        Temp_temp t = munchExp(el->head);
        if (i < 4) {
            emit(AS_Oper(String_format("mov r%d, `s0", i),
                NULL, Temp_TempList(t, NULL), NULL));
        }
        tl = Temp_TempList(t, tl);
        i++;
        el = el->tail;
    }
    while (tl) {
        emit(AS_Oper(String_format("push {`s0}"),
            NULL, Temp_TempList(tl->head, NULL), NULL));
        tl = tl->tail;
    }
}

static Temp_temp munchExp(T_exp e) {
    switch (e->kind) {
        case T_BINOP: {
            Temp_temp ret = Temp_newtemp();
            munch_binop(ret, e);
            return ret;
        }
        case T_MEM: {
            Temp_temp src = munchExp(e->u.MEM); 
            Temp_temp dst = Temp_newtemp();
            emit(AS_Oper(String_format("ldr `d0, [`s0]"),
                Temp_TempList(dst, NULL), Temp_TempList(src, NULL), NULL));
            return dst;
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
			emit(AS_Oper(String_format("ldr `d0, %s", Temp_labelstring(e->u.NAME)),
                Temp_TempList(ret, NULL), NULL, NULL));
            return ret;
        }
        case T_CONST: {
            Temp_temp ret = Temp_newtemp();
            emit(AS_Oper(String_format("mov `d0, %d", e->u.CONST), 
                Temp_TempList(ret, NULL), NULL, NULL));
            return ret;
        }
        case T_CALL: {
            Temp_temp t = munchExp(e->u.CALL.obj);
            Temp_temp func = Temp_newtemp();
            emit(AS_Oper(String_format("ldr `d0, [`s0]"),
                Temp_TempList(func, NULL), Temp_TempList(t, NULL), NULL));            
            munch_args(e->u.CALL.args);
            emit(AS_Oper(String_format("blx `s0"),
                NULL, Temp_TempList(func, NULL), NULL));
            Temp_temp ret = Temp_newtemp();
            emit(AS_Oper(String_format("mov `d0, r0"),
                Temp_TempList(ret, NULL), NULL, NULL));
			return ret;
        }
        case T_ExtCALL: {
            munch_args(e->u.ExtCALL.args);
            emit(AS_Oper(String_format("bl %s", e->u.ExtCALL.extfun),
                NULL, NULL, NULL));
            Temp_temp ret = Temp_newtemp();
            emit(AS_Oper(String_format("mov `d0, r0"),
                Temp_TempList(ret, NULL), NULL, NULL));
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
            emit(AS_Oper(String_format("b `j0"),
                NULL, NULL, AS_Targets(Temp_LabelList(s->u.JUMP.jump, NULL))));
            break;
        }
        case T_CJUMP: {
            Temp_temp s0 = munchExp(s->u.CJUMP.left), s1 = munchExp(s->u.CJUMP.right);
            emit(AS_Oper(String_format("cmp `s0, `s1"),
                NULL, Temp_TempList(s0, Temp_TempList(s1, NULL)), NULL)); 
            char *op = NULL;
            switch (s->u.CJUMP.op) {
                case T_eq: op = "beq";  break;
                case T_ne: op = "bne";  break;
                case T_lt: op = "blt"; break;
                case T_gt: op = "bgt"; break;
                case T_le: op = "ble"; break;
                case T_ge: op = "bge"; break;
                default: fprintf(stderr, "error in CJUMP!"); break;
            }
            emit(AS_Oper(String_format("%s `j0", op),
                NULL, NULL,  AS_Targets(Temp_LabelList(s->u.CJUMP.true, NULL)))); 
            emit(AS_Oper(String_format("b `j0"), 
                NULL, NULL, AS_Targets(Temp_LabelList(s->u.CJUMP.false, NULL))));
            break;
        }
        case T_MOVE: {
            if (s->u.MOVE.dst->kind == T_MEM) {
                Temp_temp src = munchExp(s->u.MOVE.src);
                Temp_temp dst = munchExp(s->u.MOVE.dst->u.MEM);
                emit(AS_Oper(String_format("str `s0, [`s1]"),
                    NULL, Temp_TempList(src, Temp_TempList(dst, NULL)), NULL));
            } else {
                Temp_temp src = munchExp(s->u.MOVE.src);
                Temp_temp dst = munchExp(s->u.MOVE.dst);
                emit(AS_Oper(String_format("mov `d0, `s0"),
                    Temp_TempList(dst, NULL), Temp_TempList(src, NULL), NULL));
            }
            break;
        }
        case T_EXP: {
            munchExp(s->u.EXP);
			break;
        }
        case T_RETURN: {
            Temp_temp ret = munchExp(s->u.EXP);
            emit(AS_Oper(String_format("mov r0, `s0"),
                NULL, Temp_TempList(ret, NULL), NULL));          
            emit(AS_Oper(String_format("mov sp, fp"),
                NULL, NULL, NULL));
            emit(AS_Oper(String_format("pop {fp}"),
                NULL, NULL, NULL));
            emit(AS_Oper(String_format("bx `s0"),
                NULL, Temp_TempList(ret_addr, NULL), NULL));          
            break;
        }
        default: fprintf(stderr, "error in munchStm!"); break;
    }
}

static AS_instrList codegen(T_stmList stmList) {
    AS_instrList list = NULL;
    T_stmList sl;
    for (sl = stmList; sl; sl = sl->tail) {
		munchStm(sl->head);
    }
	list = iList;
	iList = last = NULL;
	return list;
}

static void load_args(Temp_tempList args) {
    int i = 0;
    while (i < 4 && args) {
        emit(AS_Oper(String_format("mov `d0, r%d", i),
            Temp_TempList(args->head, NULL), NULL, NULL));
        i++;
        args = args->tail;
    }
    while (args) {
        emit(AS_Oper(String_format("ldr `d0, [fp, %d]", (i + 1) * 4),
            Temp_TempList(args->head, NULL), NULL, NULL));
        i++;
        args = args->tail;
    }
}

static AS_instrList progen(T_funcDecl fl) {
    emit(AS_Oper(String_format("%s:", fl->name),
        NULL, NULL, NULL));
    emit(AS_Oper(String_format("push {fp}"),
        NULL, NULL, NULL));
    emit(AS_Oper(String_format("mov fp, sp"),
        NULL, NULL, NULL));
    load_args(fl->args);
    ret_addr = Temp_newtemp();
    emit(AS_Oper(String_format("mov `d0, lr"),
        Temp_TempList(ret_addr, NULL), NULL, NULL));    
    AS_instrList list = iList;
	iList = last = NULL;
	return list;
}

static AS_instrList epigen(Temp_label l) {
    emit(AS_Label(String_format("%s:", Temp_labelstring(l)), l));
    emit(AS_Oper(String_format("bx `s0"), NULL, Temp_TempList(ret_addr, NULL), NULL));

    AS_instrList list = iList;
	iList = last = NULL;
	return list;
}

struct AS_prog irp2rpi(struct C_block c, T_funcDecl fl) {
/* Here you are supposed to perform instruction selection for
each block in C_block, and generate AS_blocks, one at a time.
You are also to generate the prolog and epilog.
Then package the three items into a AS_prog to return */
    AS_instrList asl1 = progen(fl);
    AS_blockList asbl = NULL, tail = NULL;
    for (C_stmListList sList = c.stmLists; sList; sList = sList->tail) {
        AS_block asb = AS_Block(codegen(sList->head));
        if (tail == NULL) {
            asbl = tail = AS_BlockList(asb, NULL);
        } else {
            tail->tail = AS_BlockList(asb, NULL);
            tail = tail->tail;
        }
    }
    AS_instrList asl3 = epigen(c.label);
    struct AS_prog ret = {.prolog = asl1, .epilog = asl3, .bl = asbl};
    return ret;
}