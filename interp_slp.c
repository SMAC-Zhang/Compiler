#include "interp_slp.h"

extern Table_ head;

// return the value of a exp
static int interp_exp(FILE *stream, A_exp exp) {
    int op1, op2;
    Table_ t;
    switch (exp->kind) {
    case A_opExp:
        // i think we don't need to care about computation priority
        op1 = interp_exp(stream, exp->u.op.left);
        op2 = interp_exp(stream, exp->u.op.right);
        switch (exp->u.op.oper) {
        case A_plus:
            return op1 + op2;
            break;
        case A_minus:
            return op1 - op2;
            break;
        case A_times:
            return op1 * op2;
            break;
        case A_div:
            return op1 / op2;
            break;
        }
        break;
    case A_eseqExp:
        interp_stmt(stream, exp->u.eseq.stm);
        return interp_exp(stream, exp->u.eseq.exp);
        break;
    case A_idExp:
        return table_lookup(exp->u.id);
        break;
    case A_numExp:
        return exp->u.num;
        break;
    }
}

static void interp_assign_stmt(FILE* stream, A_stm stm) {
    A_exp exp = stm->u.assign.exp;
    int op = interp_exp(stream, exp);
    Table_ t = Table(stm->u.assign.id, op, head);
    head = t;
}

static void interp_print_stmt(FILE *stream, A_expList expList) {
    int op;
    switch (expList->kind) {
    case A_pairExpList:
        op = interp_exp(stream, expList->u.pair.head);
        break;
    case A_lastExpList:
        op = interp_exp(stream, expList->u.last);
        break;
    }
    
    fprintf(stream, "%d\n", op);

    if (expList->kind == A_pairExpList) {
        interp_print_stmt(stream, expList->u.pair.tail);
    }
}

void interp_stmt(FILE *stream, A_stm stm) {
    if (stm == NULL) {
        return;
    }
    switch(stm->kind) {
    case A_compoundStm:
        interp_stmt(stream, stm->u.compound.stm1);
        interp_stmt(stream, stm->u.compound.stm2);
        break;
    case A_assignStm:
        interp_assign_stmt(stream, stm);
        break;
    case A_printStm:
        interp_print_stmt(stream, stm->u.print.exps);
        break;
    }
}
