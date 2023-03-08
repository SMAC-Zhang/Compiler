#include "compile_slp.h"

static int register_num = 1;
extern Table_ head;


// if exp is an opExp or eseqExp, this function will be called
// return a register_id that stores the result of the exp
static int compile_exp(FILE *stream, A_exp exp) {
    int op1, op2;
    Table_ t;
    switch (exp->kind) {
    case A_opExp:
        // i think we don't need to care about computation priority
        op1 = compile_exp(stream, exp->u.op.left);
        op2 = compile_exp(stream, exp->u.op.right);

        t = Table("const\0", register_num++, head);
        head = t;
        switch (exp->u.op.oper) {
        case A_plus:
            fprintf(stream, "    %%%d = add i32 %%%d, %%%d\n", t->value, op1, op2);
            break;
        case A_minus:
            fprintf(stream, "    %%%d = sub i32 %%%d, %%%d\n", t->value, op1, op2);           
            break;
        case A_times:
            fprintf(stream, "    %%%d = mul i32 %%%d, %%%d\n", t->value, op1, op2);           
            break;
        case A_div:
            fprintf(stream, "    %%%d = sdiv i32 %%%d, %%%d\n", t->value, op1, op2);           
            break;
        }
        return t->value;
        break;
    case A_eseqExp:
        compile_stmt(stream, exp->u.eseq.stm);
        return compile_exp(stream, exp->u.eseq.exp);
        break;
    case A_idExp:
        return table_lookup(exp->u.id);
        break;
    case A_numExp:
        t = Table("const\0", register_num++, head);
        head = t;
        fprintf(stream, "    %%%d = add i32 %d, 0\n", t->value, exp->u.num);
        return t->value;
        break;
    }
}

static void compile_assign_stmt(FILE *stream, A_stm stm) {
    A_exp exp = stm->u.assign.exp;
    int op = compile_exp(stream, exp);
    Table_ t = Table(stm->u.assign.id, register_num++, head);
    head = t;
    fprintf(stream, "    %%%d = add i32 %%%d, 0\n", t->value, op);
}

static void compile_print_stmt(FILE *stream, A_expList expList) {
    int op;
    switch (expList->kind) {
    case A_pairExpList:
        op = compile_exp(stream, expList->u.pair.head);
        break;
    case A_lastExpList:
        op = compile_exp(stream, expList->u.last);
        break;
    }
    
    fprintf(stream, "    call void @putint(i32 %%%d)\n", op);
    fprintf(stream, "    call void @putch(i32 10)\n");

    if (expList->kind == A_pairExpList) {
        compile_print_stmt(stream, expList->u.pair.tail);
    }
}

void compile_stmt(FILE *stream, A_stm stm) {
    if (stm == NULL) {
        return;
    }
    switch(stm->kind) {
    case A_compoundStm:
        compile_stmt(stream, stm->u.compound.stm1);
        compile_stmt(stream, stm->u.compound.stm2);
        break;
    case A_assignStm:
        compile_assign_stmt(stream, stm);
        break;
    case A_printStm:
        compile_print_stmt(stream, stm->u.print.exps);
        break;
    }
}