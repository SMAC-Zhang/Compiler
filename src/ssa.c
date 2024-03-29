#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"
#include "graph.h"
#include "temp.h"
#include "flowgraph.h"
#include "ssa.h"
#include "assemblock.h"
#include "liveness.h"

// 求并集
static bool* get_union(bool *a, bool *b, int len) {
    bool *c = checked_malloc(len * sizeof(bool));
    memset(c, 0, len * sizeof(bool));
    for (int i = 0; i < len; ++i) {
        c[i] = a[i] || b[i];
    }
    return c;
}

// 求交集
static bool* get_inter(bool *a, bool *b, int len) {
    bool *c = checked_malloc(len * sizeof(bool));
    memset(c, 0, len * sizeof(bool));
    for (int i = 0; i < len; ++i) {
        c[i] = a[i] && b[i];
    }
    return c;
}

// 求差集
static bool* get_diff(bool* a, bool* b, int len) {
    bool *c = checked_malloc(len * sizeof(bool));
    memset(c, 0, len * sizeof(bool));
    for (int i = 0; i < len; ++i) {
        c[i] = a[i] && !b[i];
    }
    return c;
}

// 判断两个集合是否相等
static bool is_same(bool* a, bool* b, int len) {
    for (int i = 0; i < len; ++i) {
        if (a[i] != b[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

// 数据结构栈
#define STACK_SIZE 1024
typedef struct stack_ *stack;
struct stack_ {
    int size;
    int data[STACK_SIZE];
};

static stack Stack() {
    stack s = checked_malloc(sizeof *s);
    s->size = -1;
}

static void Stack_Push(stack s, int x) {
    if (s->size + 1 == STACK_SIZE) {
        fprintf(stderr, "STACK IS FULL\n");
        exit(1);
    }
    s->size++;
    s->data[s->size] = x;
}

static void Stack_Pop(stack s) {
    if (s->size == -1) {
        fprintf(stderr, "STACK IS EMPTY\n");
        exit(1);
    }
    s->size--;
}

static int Stack_top(stack s) {
    if (s->size == -1) {
        fprintf(stderr, "STACK IS EMPTY\n");
        exit(1);
    }
    return s->data[s->size];
}

static int Stack_size(stack s) {
    return s->size + 1;
}

// 每个变量都要一个栈和一个结点域
typedef struct var_Struct_ *var_Struct;
struct var_Struct_ {
    stack st;
    bool* defsites;
};

static var_Struct Var_Struct(int node_num) {
    var_Struct p = checked_malloc(sizeof *p);
    p->st = Stack();
    p->defsites = checked_malloc(node_num * sizeof(bool));
    memset(p->defsites, 0, node_num * sizeof(bool));
    return p;
}

// 每个结点需要记录Aorig， Aphi, DF, D
typedef struct node_Struct_ *node_Struct;
struct node_Struct_ {
    bool* orig;
    bool* phi;
    bool* DF; // 必经结点边界
    bool* D; // 必经结点
    bool* son; // 必经结点树的儿子
    bool* In;
    bool* Out;
    bool* Use;
    bool* Def;
    G_node idom;
    G_node self;
};

static node_Struct Node_Struct(G_node n, int var_num, int node_num) {
    node_Struct p = checked_malloc(sizeof *p);

    p->orig = checked_malloc(var_num * sizeof(bool));
    memset(p->orig, 0, var_num * sizeof(bool));
    p->In = checked_malloc(var_num * sizeof(bool));
    memset(p->In, 0, var_num * sizeof(bool));
    p->Out = checked_malloc(var_num * sizeof(bool));
    memset(p->Out, 0, var_num * sizeof(bool));
    p->Def = checked_malloc(var_num * sizeof(bool));
    memset(p->Def, 0, var_num * sizeof(bool));
    p->Use = checked_malloc(var_num * sizeof(bool));
    memset(p->Use, 0, var_num * sizeof(bool));
    
    AS_block asb = G_nodeInfo(n);
    AS_instrList instrs = asb->instrs;
    while (instrs) {
        AS_instr instr = instrs->head;
        Temp_tempList def = NULL, use = NULL;
        if (instr->kind == I_OPER) {
            def = instr->u.OPER.dst;
            use = instr->u.OPER.src;
        } else if (instr->kind == I_MOVE) {
            def = instr->u.MOVE.dst;
            use = instr->u.OPER.src;   
        } else {
            // 对LABEL语句应该什么都不做？
        }
        while (def) {
            p->orig[temp_id(def->head)] = TRUE;
            p->Def[temp_id(def->head)] = TRUE;
            def = def->tail;
        }
        while (use) {
            if (p->Def[temp_id(use->head)] == FALSE) {
                p->Use[temp_id(use->head)] = TRUE;
            }
            use = use->tail;
        }
        instrs = instrs->tail;
    }

    p->phi = checked_malloc(var_num * sizeof(bool));
    memset(p->phi, 0, var_num * sizeof(bool));
    // 在计算DF时会赋值
    p->DF = NULL; 
    // p->DF = checked_malloc(node_num * sizeof(bool));
    // memset(p->DF, 0, node_num * sizeof(bool));
    p->D = NULL;
    // p->D = checked_malloc(node_num * sizeof(bool));
    // memset(p->D, 0, node_num * sizeof(bool));
    // p->D[G_id(n)] = TRUE; // 自己是自己的必经结点
    p->son = checked_malloc(node_num * sizeof(bool));
    memset(p->son, 0, node_num * sizeof(bool));

    p->idom = NULL;
    p->self = n;
    return p;
}

static var_Struct* var_set;
static node_Struct* node_set;
static int node_num;
static int var_num;

// 初始化需要的数据结构
static void init_struct(G_nodeList gl) {
    // 初始化变量数据结构
    var_set = checked_malloc(var_num * sizeof(var_Struct));
    for (int i = 0; i < var_num; ++i) {
        var_set[i] = Var_Struct(node_num);
    }

    // 初始化结点数据结构
    node_set = checked_malloc(node_num * sizeof(node_Struct));
    memset(node_set, 0, node_num * sizeof(node_Struct));
    while (gl) {
        G_node n = gl->head;
        node_set[G_id(n)] = Node_Struct(n, var_num, node_num);
        gl = gl->tail;
    }
}

// 计算活跃性迭代
static bool mylivenessIteration() {
    bool changed = FALSE;
    for (int i = 0; i < node_num; ++i) {
        bool *in = get_union(node_set[i]->Use, get_diff(node_set[i]->Out, node_set[i]->Def, var_num), var_num);

        G_nodeList succ = G_succ(node_set[i]->self);
        bool* out = checked_malloc(var_num * sizeof (bool));
        memset(out, 0, var_num * sizeof(bool));
        while (succ) {
            out = get_union(out, node_set[G_id(succ->head)]->In, var_num);
            succ = succ->tail;
        }
        if (!is_same(node_set[i]->In, in, var_num) || !is_same(node_set[i]->Out, out, var_num)) {
            changed = TRUE;
        }
        node_set[i]->In = in;
        node_set[i]->Out = out;
    }
    return changed;
}

// 活跃分析
static void myliveness() {
    bool changed = TRUE;
    while (changed) {
        changed = mylivenessIteration();
    }
}

// 拓扑排序辅助用的dfs
static void topo_dfs(int* N, bool* mark, int* sorted, G_node i) {
    if (mark[G_id(i)] == FALSE) {
        mark[G_id(i)] = TRUE;
        G_nodeList succ = G_succ(i);
        while (succ) {
            topo_dfs(N, mark, sorted, succ->head);
            succ = succ->tail;
        }
        sorted[*N - 1] = G_id(i);
        (*N)--;
    }
}

// 算法17-1, 拓扑排序
static void topological_sort(int* sorted, G_node root) {
    bool* mark = checked_malloc(node_num * sizeof(bool));
    memset(mark, 0, node_num * sizeof(bool));
    int temp_node_num = node_num;
    topo_dfs(&temp_node_num, mark, sorted, root);
}

// 18.1.1, 计算必经结点树
static void compute_dom(G_nodeList gl) {
    // 初始化
    for (int i = 0; i < node_num; ++i) {
        node_set[i]->D = checked_malloc(node_num * sizeof(bool));
        if (i == 0) {
            memset(node_set[i]->D, 0, node_num * sizeof(bool));
            node_set[i]->D[0] = TRUE;
        } else {
            memset(node_set[i]->D, 1, node_num * sizeof(bool));
        }
    }

    // 开始迭代
    while (TRUE) {
        bool nice = TRUE;
        for (int i = 1; i < node_num; ++i) {
            G_nodeList pred = G_pred(node_set[i]->self);
            bool *S = checked_malloc(node_num * sizeof(bool));
            memset(S, 1, node_num * sizeof(bool));
            while (pred) {
                int pid = G_id(pred->head);
                S = get_inter(node_set[pid]->D, S, node_num);
                pred = pred->tail;
            }

            bool *SS = checked_malloc(node_num * sizeof(bool));
            memset(SS, 0, node_num * sizeof(bool));
            SS[i] = TRUE;
            bool *ret = get_union(SS, S, node_num);
            if (!is_same(ret, node_set[i]->D, node_num)) {
                node_set[i]->D = ret;
                nice = FALSE;
            }
        }
        if (nice) {
            break;
        }
    }
}

// 18.1.2, 计算直接必经结点
static void compute_idom() {
    for (int i = 1; i < node_num; ++i) {
        for (int j = 0; j < node_num; ++j) {
            // 1.idom(n)和n不是同一个结点
            if (j == i) {
                continue;
            }
            // 2.idom(n)是n的必经结点
            if (node_set[i]->D[j]) {
                // 3.idom(n)不是其他必经结点的必经结点
                bool nice = TRUE;
                for (int k = 0; k < node_num; ++k) {
                    if (k != i && k != j && node_set[i]->D[k]) {
                        if (node_set[k]->D[j]) {
                            nice = FALSE;
                            break;
                        }
                    }
                }
                if (nice) {
                    node_set[i]->idom = node_set[j]->self;
                    node_set[j]->son[i] = TRUE;
                    break;
                }
            }
        }
    }
}

// 19.1.2, 计算必经结点边界
static void computeDF(G_node n) {
    // 计算DFlocal[n]
    G_nodeList succ = G_succ(n);
    bool* S = checked_malloc(node_num * sizeof(bool));
    memset(S, 0, node_num * sizeof(bool));
    while (succ) {
        G_node y = succ->head;
        G_node idom = node_set[G_id(y)]->idom;
        if (idom && G_id(idom) != G_id(n)) {
            S[G_id(y)] = TRUE;
        }
        succ = succ->tail;
    }
    
    // 计算DFup[c]
    for (int c = 0; c < node_num; ++c) {
        if (node_set[G_id(n)]->son[c]) { // 对于n在必经结点树中的每个儿子c
            computeDF(node_set[c]->self);
            for (int w = 0; w < node_num; ++w) {
                if (node_set[c]->DF[w]) { // 对DF[c]中的每个元素w
                    if (node_set[w]->D[G_id(n)] == FALSE || G_id(n) == w) {
                        S[w] = TRUE;
                    }
                }
            }
        }
    }
    node_set[G_id(n)]->DF = S;
}

// 构造phi语句
static AS_instr make_phi(int var, int node) {
    char s[1024] = {'\0'};
    strcat(s, String_format("%%`d0 = phi i64 "));

    Temp_temp t = get_temp(temp_id2name(var));
    int num = 0;
    Temp_tempList dst = Temp_TempList(t, NULL);
    Temp_tempList src = NULL;
    Temp_labelList tl = NULL, tl_last = NULL;
    G_nodeList pred = G_pred(node_set[node]->self);
    while (pred) {
        if (pred->tail) {
            strcat(s, String_format("[%%`s%d, %%`j%d], ", num, num));
        } else {
            strcat(s, String_format("[%%`s%d, %%`j%d]", num, num));
        }
        src = Temp_TempList(t, src);
        AS_block asb = G_nodeInfo(pred->head);
        Temp_label label_t = asb->label;
        if (tl_last == NULL) {
            tl = tl_last = Temp_LabelList(label_t, NULL);
        } else {
            tl_last->tail = Temp_LabelList(label_t, NULL);
            tl_last = tl_last->tail;
        }
        num++;
        pred = pred->tail;
    }
    return AS_Oper(String(s), dst, src, AS_Targets(tl));
}

// 算法19-1, 插入phi函数
static void place_phi_functions(G_nodeList gl) {
    for (int n = 0; n < node_num; ++n) { // 对每个结点n
        bool *orig = node_set[n]->orig;
        for (int a = 0; a < var_num; ++a) {
            if (orig[a] == TRUE) { // 对Aorig[n]中的每个变量a
                var_set[a]->defsites[n] = TRUE;
            }
        }
    }

    for (int a = 0; a < var_num; ++a) { // 对每个变量a
        bool* W = var_set[a]->defsites;
        int num = 0;
        for (int i = 0; i < node_num; ++i) {
            num += (W[i] == TRUE);
        }
        while (num > 0) { // 当W非空
            for (int n = 0; n < node_num; ++n) { 
                if (W[n] == TRUE) { // 对W中的某个结点n
                    num--;  W[n] = FALSE; // 删除n
                    bool *DFn = node_set[n]->DF;
                    for (int Y = 0; Y < node_num; ++Y) {
                        if (DFn[Y] == TRUE) { // 对DF[n]中的每个Y
                            // a 需要在结点Y上是livein的
                            if (node_set[Y]->phi[a] == FALSE && node_set[Y]->In[a]) {
                                AS_instr phi_instr = make_phi(a, Y);
                                AS_block asb = G_nodeInfo(node_set[Y]->self);
                                asb->instrs->tail = AS_InstrList(phi_instr, asb->instrs->tail);
                                node_set[Y]->phi[a] = TRUE;
                                if (node_set[Y]->orig[a] == FALSE && W[Y] == FALSE) {
                                    W[Y] = TRUE;
                                    num++;
                                }
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
}

// 判断是否是phi语句
static bool is_phi(AS_instr instr) {
    string ass = NULL;
    if (instr->kind == I_OPER) {
        ass = instr->u.OPER.assem;
    } else {
        return FALSE;
    }
    for (int i = 0; i < strlen(ass); ++i) {
        if (ass[i] == '[') {
            return TRUE;
        }
    }
    return FALSE;
}

// 找到n是第几个前驱或后继
// 从0开始数
static int find_seq(G_nodeList gl, G_node n) {
    int ret = 0;
    while (gl) {
        if (G_id(n) == G_id(gl->head)) {
            return ret;
        }
        ret++;
        gl = gl->tail;
    }
    fprintf(stderr, "Not Find!!!\n");
}

// 算法19-2 重命名变量
static void rename_var(int n) {
    // 1.对于n中的每一个语句S
    AS_block asb = G_nodeInfo(node_set[n]->self);
    AS_instrList instrs = asb->instrs;
    AS_instrList heads = instrs;
    while (instrs) {
        AS_instr instr = instrs->head;
        if (!is_phi(instr)) {
            Temp_tempList use = NULL;
            if (instr->kind == I_OPER) {
                use = instr->u.OPER.src;
            } else if (instr->kind == I_MOVE) {
                use = instr->u.MOVE.src;  
            } else {
                // 对LABEL语句应该什么都不做？
            }
            while (use) {
                use->head = get_temp(Stack_top(var_set[temp_origin(use->head)]->st)); 
                use = use->tail;
            }    
        }
        // 对S中的每一个定值
        Temp_tempList def = NULL;
        if (instr->kind == I_OPER) {
            def = instr->u.OPER.dst;
        } else if (instr->kind == I_MOVE) {
            def = instr->u.MOVE.dst;   
        } else {
            // 对LABEL语句应该什么都不做？
        }
        while (def) {
            if (Stack_size(var_set[temp_origin(def->head)]->st) > 0) {
                Temp_temp t = Temp_newtemp();
                set_origin(t, temp_origin(def->head));
                Stack_Push(var_set[temp_origin(def->head)]->st, temp_id2name(temp_id(t)));
                def->head = t;
            } else {
                Stack_Push(var_set[temp_origin(def->head)]->st, temp_id2name(temp_id(def->head)));
            }
            def = def->tail;
        }    
        instrs = instrs->tail;
    }
    
    // 2.对n的每一个后继Y
    G_nodeList succ = G_succ(node_set[n]->self);
    while (succ) {
        G_node Y = succ->head;
        // 设n是Y的第j个前驱
        int j = find_seq(G_pred(Y), node_set[n]->self);
        // 对Y中的每一个phi函数
        AS_block asb_Y = G_nodeInfo(node_set[G_id(Y)]->self);
        AS_instrList instrs_Y = asb_Y->instrs;
        while (instrs_Y) {
            AS_instr S = instrs_Y->head;
            if (is_phi(S)) {
                Temp_tempList use = S->u.OPER.src;
                for (int i = 0; i < j; ++i) {
                    use = use->tail;
                }
                use->head = get_temp(Stack_top(var_set[temp_origin(use->head)]->st)); 
            }
            instrs_Y = instrs_Y->tail;
        }
        succ = succ->tail;
    }

    // 3.对n的每一个儿子X
    for (int x = 0; x < node_num; ++x) {
        if (node_set[n]->son[x]) {
            rename_var(x);
        }
    }
    
    // 4.对原来的S中的某个变量a的每一个定值
    instrs = heads;
    while (instrs) {
        AS_instr instr = instrs->head;
        Temp_tempList def = NULL;
        if (instr->kind == I_OPER) {
            def = instr->u.OPER.dst;
        } else if (instr->kind == I_MOVE) {
            def = instr->u.MOVE.dst;  
        } else {
            // 对LABEL语句应该什么都不做？
        }
        while (def) {
            Stack_Pop(var_set[temp_origin(def->head)]->st);
            def = def->tail;
        }    
        instrs = instrs->tail;
    }
}

void ssa_form(G_nodeList gl) {
    var_num = temp_num();
    // 统计结点数量
    node_num = G_nodecount(gl->head);

    // 初始化数据结构
    init_struct(gl);
    // 计算活跃性
    myliveness();

    // ssa
    compute_dom(gl);
    compute_idom();
    computeDF(gl->head);
    place_phi_functions(gl);
    rename_var(0);
}

// 打印活跃图
// debug 用
static void print_liveness() {
    for (int i = 0; i < node_num; ++i) {
        fprintf(stderr, "node %d: \n", i);
        fprintf(stderr, "   Use: ");
        for (int j = 0; j < var_num; ++j) {
            if (node_set[i]->Use[j]) {
                fprintf(stderr, "%d, ", temp_id2name(j));
            }
        }
        fprintf(stderr, "\n");

        fprintf(stderr, "   Def: ");
        for (int j = 0; j < var_num; ++j) {
            if (node_set[i]->Def[j]) {
                fprintf(stderr, "%d, ", temp_id2name(j));
            }
        }
        fprintf(stderr, "\n");

        fprintf(stderr, "   In: ");
        for (int j = 0; j < var_num; ++j) {
            if (node_set[i]->In[j]) {
                fprintf(stderr, "%d, ", temp_id2name(j));
            }
        }
        fprintf(stderr, "\n");

        fprintf(stderr, "   Out: ");
        for (int j = 0; j < var_num; ++j) {
            if (node_set[i]->Use[j]) {
                fprintf(stderr, "%d, ", temp_id2name(j));
            }
        }
        fprintf(stderr, "\n");
    }
}