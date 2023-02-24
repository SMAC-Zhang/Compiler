#include "interp.h"

static int register_num = 1;
extern Table_ head;


// if exp is an opExp or eseqExp, this function will be called
// return a register_id that stores the result of the exp
static int interp_exp(FILE *stream, A_exp exp) {
    A_exp left, right;
    switch (exp->kind) {
        case A_opExp:
            // i think we don't need to care about computation priority
            left = exp->u.op.left, right = exp->u.op.right;
            int op1, op2;
            if (left->kind == A_opExp || left->kind == A_eseqExp) {
                op1 = interp_exp(stream, left);
            } else if (left->kind == A_idExp) {
                op1 = find_in_table(left->u.id);
            } else {
                op1 = -1;
            }
            if (right->kind == A_opExp || right->kind == A_eseqExp) {
                op2 = interp_exp(stream, right);
            } else if (right->kind == A_idExp) {
                op2 = find_in_table(right->u.id);
            } else {
                op2 = -1;
            }

            Table_ t = Table("const\0", register_num++, head);
            head = t;
            fprintf(stream, "    %%%d = ", t->value);
            switch (exp->u.op.oper) {
            case A_plus:
                fprintf(stream, "add i32 ");
                break;
            case A_minus:
                fprintf(stream, "sub i32 ");            
                break;
            case A_times:
                fprintf(stream, "mul i32 ");
                break;
            case A_div:
                fprintf(stream, "sdiv i32 ");
                break;
            }
            if (op1 == -1) {
                fprintf(stream, "%d, ", left->u.num);
            } else {
                fprintf(stream, "%%%d, ", op1);
            }
            if (op2 == -1) {
                fprintf(stream, "%d\n", right->u.num);
            } else {
                fprintf(stream, "%%%d\n", op2);
            }
            return t->value;
            break;
        case A_eseqExp:
            interp_stmt(stream, exp->u.eseq.stm);
            return interp_exp(stream, exp->u.eseq.exp);
            break;
        case A_idExp:
            return find_in_table(exp->u.id);
        case A_numExp:
            return -1;
    }
}

static void interp_assign_stmt(FILE *stream, A_stm stm) {
    A_exp exp = stm->u.assign.exp;
    int op = interp_exp(stream, exp);
    Table_ t = Table(stm->u.assign.id, register_num++, head);
    head = t;
    if (op != -1) {
        fprintf(stream, "    %%%d = add i32 %%%d, 0\n", t->value, op);
    } else {
        if (exp->kind == A_eseqExp) {
            fprintf(stream, "    %%%d = add i32 %d, 0\n", t->value, exp->u.eseq.exp->u.num);
        } else {
            fprintf(stream, "    %%%d = add i32 %d, 0\n", t->value, exp->u.num);            
        }
    }
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

    if (op != -1) {
        fprintf(stream, "    call void @putint(i32 %%%d)\n", op);
    } else {
        fprintf(stream, "    call void @putint(i32 %d)\n", expList->u.pair.head->u.num);
    }
    fprintf(stream, "    call void @putch(i32 10)\n");

    if (expList->kind == A_pairExpList) {
        interp_print_stmt(stream, expList->u.pair.tail);
    }
}

void interp_stmt(FILE *stream, A_stm stm) {
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