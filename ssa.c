#include <stdio.h>

#include "util.h"
#include "graph.h"
#include "temp.h"
#include "flowgraph.h"
#include "ssa.h"

// 数据结构栈
#define STACK_SIZE 256
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

// 每个变量都要一个栈和一个结点域
typedef struct var_Struct_ *var_Struct;
struct var_Struct_ {
    stack data;
    G_nodeList defsites;
};

static var_Struct Var_Struct() {
    var_Struct p = checked_malloc(sizeof *p);
    p->data = Stack();
    p->defsites = NULL;
    return p;
}

// 每个结点需要记录Aorig， Aphi, DF
typedef struct node_Struct_ *node_Struct;
static struct node_Struct_ {
    Temp_tempList orig;
    Temp_tempList phi;
    G_nodeList DF;
};

static var_Struct* var_set;
static node_Struct* node_set;

static node_Struct Node_Struct(G_node n) {
    node_Struct p = checked_malloc(sizeof *p);
    p->orig = FG_def(n);
    p->phi = NULL;
    p->DF = NULL;
    return p;
}

// 在集合A中寻找变量a
static bool find_in_A(Temp_tempList A, int a) {
    if (A)
    while (A) {
        if (temp_id(A->head) == a) {
            return TRUE;
        }
        A = A->tail;
    }
    return FALSE;
}

// 初始化需要的数据结构
static void init_struct(int var_num, G_nodeList gl) {
    // 初始化变量数据结构
    var_set = checked_malloc(var_num * sizeof(var_Struct));
    for (int i = 0; i < var_num; ++i) {
        var_set[i] = Var_Struct();
    }

    // 初始化结点数据结构
    // 统计结点数量
    int node_num = 0;
    while (gl) {
        gl = gl->tail;
        node_num++;
    }
    if (node_num == 0) {
        return;
    }
    node_set = checked_malloc(node_num * sizeof(node_Struct));
    for (G_node n = gl->head; gl; gl = gl->tail, n = gl->head) {
        node_set[G_id(n)] = Node_Struct(n);
    }
}

// 计算必经结点边界
static void computeDF(G_nodeList gl) {

}

// 算法19-1, 插入phi函数
static void place_phi_functions(G_nodeList gl) {
    while (gl) {
        G_node n = gl->head;
        Temp_tempList ori = FG_def(n);
        while (ori) {
            Temp_temp t = ori->head;
            G_nodeList defsites = var_set[temp_id(t)]->defsites;
            defsites = G_NodeList(n, defsites);
            ori = ori->tail;
        }
        gl = gl->tail;
    }

    int var_num = temp_num();
    for (int i = 0; i < var_num; ++i) {
        G_nodeList W = var_set[i]->defsites;
        while (W != NULL) {
            G_node n = W->head;
            W = W->tail;
            G_nodeList DF = node_set[G_id(n)]->DF;
            while (DF) {
                G_node Y = DF->head;
                if (!find_in_A(node_set[G_id(n)]->phi, i)) {
                    
                }
                DF = DF->tail;
            }
        }
    }
}

// 算法19-2 重命名变量
static void rename_var() {

}

void ssa_form(G_nodeList gl) {
    int var_num = temp_num();
    // 初始化数据结构
    init_struct(var_num, gl);
    
    // ssa
    computeDF(gl);
    place_phi_functions(gl);
    rename_var();
}