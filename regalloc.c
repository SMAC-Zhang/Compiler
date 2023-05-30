#include <assert.h>

#include "regalloc.h"
#include "ig.h"
#define REG_COUNT 8

struct COL_result {
    Temp_map coloring;
    Temp_tempList spills;
};

static void init_color(Temp_map coloring) {
    for (int i = 0; i < 4; ++i) {
        Temp_temp t = get_rtemp(i);
        Temp_enter(coloring, t, String_format("r%d", i));
        G_node n = Look_ig(t);
        n->reg = i;
    }
    for (int i = 8; i <= 10; ++i) {
        Temp_temp t = get_rtemp(i);
        Temp_enter(coloring, t, String_format("r%d", i));
    }
}

// 计算度数
static int compute_degree(G_node n) {
    G_nodeList pred = G_pred(n);
    G_nodeList succ = G_succ(n);
    int degree = 0;
    while (pred) {
        if (pred->head->simplified == FALSE) {
            degree++;
        }
        pred = pred->tail;
    }
    while (succ) {
        if (succ->head->simplified == FALSE) {
            degree++;
        }
        succ = succ->tail;
    }
    return degree;
}

// 简化冲突图
static Temp_tempList simplified(G_nodeList ig, Temp_tempList stack) {
    while (ig) {
        G_node n = ig->head;
        if (n->simplified) {
            ig = ig->tail;
            continue;
        }

        if (compute_degree(n) < REG_COUNT) {
            n->simplified = TRUE;
            stack = Temp_TempList((Temp_temp)G_nodeInfo(n), stack);
            return stack;
        }
        ig = ig->tail;
    }

    return NULL;
}

// 结点溢出
static Temp_tempList spill(G_nodeList ig, Temp_tempList spills) {
    G_node spilled = NULL;
    int degree = -1;
    while (ig) {
        if (!ig->head->simplified && ig->head->reg == -1) {
            int d = compute_degree(ig->head);
            if (degree < d) {
                degree = d;
                spilled = ig->head;
            }
        }
        ig = ig->tail;
    }
    if (spilled == NULL) {
        return NULL;
    }
    spilled->simplified = TRUE;
    spills = Temp_TempList((Temp_temp)G_nodeInfo(spilled), spills);

    return spills;
}

// 为simple的结点染色
static void color(Temp_tempList stack, Temp_map coloring) {
    while (stack) {
        bool vis[8] = {0};
        G_node n = Look_ig(stack->head);
        if (n->reg != -1) {
            stack = stack->tail;
            continue;
        }
        G_nodeList pred = G_pred(n);
        G_nodeList succ = G_succ(n);
        while (pred) {
            if (pred->head->reg != -1) {
                vis[pred->head->reg] = TRUE;
            }
            pred = pred->tail;
        }
        while (succ) {
            if (succ->head->reg != -1) {
                vis[succ->head->reg] = TRUE;
            }
            succ = succ->tail;
        }

        int reg = 0;
        while (reg < REG_COUNT && vis[reg]) {
            reg++;
        }
        assert(reg < REG_COUNT);
        n->reg = reg;

        Temp_enter(coloring, stack->head, String_format("r%d", reg));
        stack = stack->tail;
    }
}

static struct COL_result COL_color(G_nodeList ig) {
    Temp_map coloring = Temp_empty();
    init_color(coloring);
    Temp_tempList stack = NULL, spills = NULL;

    int count = G_nodecount(ig->head);
    while (TRUE) {
        while (TRUE) {
            Temp_tempList new_stack = simplified(ig, stack);
            if (new_stack) {
                stack = new_stack;
                count--;
            } else {
                break;
            }
        }
        Temp_tempList new_spills = spill(ig, spills);
        if (new_spills) {
            spills = new_spills;
            count--;
        } else {
            break;
        }
    }
    assert(count == 0);

    color(stack, coloring);

    struct COL_result ret = {.coloring = coloring, .spills = spills};
    return ret;
}

// 判断temp是否在spill栈中
static bool in_spills(Temp_tempList spills, Temp_temp t) {
    while (spills) {
        if (temp_id(spills->head) == temp_id(t)) {
            return TRUE;
        }
        spills = spills->tail;
    }
    return FALSE;
}

// 为溢出添加指令
static AS_instrList spilled(Temp_map coloring, Temp_tempList spills, AS_instrList il) {
    // 为spill变量预留位置
    Temp_tempList t_spills = spills;
    int spill_stack = 0;
    while (t_spills) {
        Look_ig(t_spills->head)->reg = spill_stack++;
        t_spills = t_spills->tail;
    }

    // 移动栈指针
    AS_instrList pre = il;
    for (int i = 0; i < 3; ++i) {
        il = il->tail;
        if (pre->tail != il) {
            pre = pre->tail;
        }
    }
    il->tail = AS_InstrList(AS_Oper(String_format("    add sp, sp, #%d", -spill_stack * 4), NULL, NULL, NULL), il->tail);

    // 添加指令
    while (il) {
        AS_instr asi = il->head;
        Temp_tempList dst = NULL, src = NULL;
        if (asi->kind == I_OPER) {
            dst = asi->u.OPER.dst;
            src = asi->u.OPER.src;
        } else if (asi->kind == I_MOVE) {
            dst = asi->u.MOVE.dst;
            src = asi->u.MOVE.src;
        }
        
        int reg = 8;
        while (dst) {
            if (in_spills(spills, dst->head)) {
                G_node dst_n = Look_ig(dst->head);
                dst->head = get_rtemp(reg);
                il->tail = AS_InstrList(AS_Oper(String_format("    str r%d, [sp, #%d]", reg, (dst_n->reg + 1) * 4), NULL, NULL, NULL), il->tail);
                reg++;
            }
            dst = dst->tail;
        }

        while (src) {
            if (in_spills(spills, src->head)) {
                G_node src_n = Look_ig(src->head);
                AS_instr pop = AS_Oper(String_format("    ldr r%d, [sp, #%d]", reg, (src_n->reg + 1) * 4), NULL, NULL, NULL);
                src->head = get_rtemp(reg);
                AS_instr push = AS_Oper(String_format("    str r%d, [sp, #%d]", reg, (src_n->reg + 1) * 4), NULL, NULL, NULL);
                assert(pre != il);
                pre->tail = AS_InstrList(pop, il);
                pre = pre->tail;
                il->tail = AS_InstrList(push, il->tail);
                reg++;
            }
            src = src->tail;
        }
        pre = pre->tail;
        il = il->tail;
    }
}

// 判断temp是否在冲突图内
static bool in_ig(G_nodeList ig, Temp_temp t) {
    while (ig) {
        if ((Temp_temp)G_nodeInfo(ig->head) == t) {
            return TRUE;
        }
        ig = ig->tail;
    }
    return FALSE;
}

// 将不发生冲突的temp直接改为r0
static void modify_friend(Temp_map coloring, G_nodeList ig, AS_instrList il) {
    while (il) {
        AS_instr asi = il->head;
        Temp_tempList dst = NULL, src = NULL;
        if (asi->kind == I_OPER) {
            dst = asi->u.OPER.dst;
            src = asi->u.OPER.src;
        } else if (asi->kind == I_MOVE) {
            dst = asi->u.MOVE.dst;
            src = asi->u.MOVE.src;
        } else {
            il = il->tail;
            continue;
        }
        while (dst) {
            if (!in_ig(ig, dst->head)) {
                Temp_enter(coloring, dst->head, String("r0"));
            }
            dst = dst->tail;
        }

        while (src) {
            if (!in_ig(ig, src->head)) {
                Temp_enter(coloring, src->head, String("r0"));
            }
            src = src->tail;
        }

        il = il->tail;
    }
}

Temp_map RA_regAlloc(G_nodeList ig, AS_instrList il) {
    struct COL_result cr = COL_color(ig);
    AS_instrList ill = spilled(cr.coloring, cr.spills, il);
    //modify_friend(cr.coloring, ig, il);
    return cr.coloring;
}