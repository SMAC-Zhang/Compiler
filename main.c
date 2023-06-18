#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "printast.h"
#include "y.tab.h"
#include "semantic.h"
#include "translate.h"
#include "pr_tree_readable.h"
#include "printtreep.h"
#include "canon.h"
#include "pr_linearized.h"
#include "assem.h"
#include "assemblock.h"
#include "graph.h"
#include "bg.h"
#include "liveness.h"
#include "flowgraph.h"
#include "ig.h"

#ifdef LLVM
#include "codegen.h"
#include "ssa.h"
#else
#include "irp2rpi.h"
#include "regalloc.h"
#endif

// AST树的根节点。
A_prog root;

// 数据流图的打印函数
void show(FILE* out, AS_instr ins) {
    FG_Showinfo(out, ins, Temp_name());
}

// 打印arm所需要的头
void print_head(FILE* out) {
	fprintf(out, ".balign 4\n");
	fprintf(out, ".global main\n");
	fprintf(out, ".section .text\n\n");
}

// 打印arm所需要的尾
void print_tail(FILE* out) {
	fprintf(out, ".global _sysy_stoptime\n");
	fprintf(out, ".global _sysy_starttime\n");
	fprintf(out, ".global putarray\n");
	fprintf(out, ".global putchar\n");
	fprintf(out, ".global putint\n");
	fprintf(out, ".global getarray\n");
	fprintf(out, ".global getch\n");
	fprintf(out, ".global getint\n");
	fprintf(out, ".global malloc\n\n");
}

// 打印llvm所需要的函数声明
void print_declare(FILE* out) {
	fprintf(out, "declare ptr @malloc(i64)\n");
	fprintf(out, "declare ptr @putarray(i64)\n");
	fprintf(out, "declare ptr @putint(i64)\n");
	fprintf(out, "declare ptr @putch(i64)\n");
	fprintf(out, "declare ptr @getarray()\n");
	fprintf(out, "declare ptr @getch()\n");
	fprintf(out, "declare ptr @getint()\n");
}

// 从带fmj后缀的文件名中剥离出前缀
string stripFilename(string filename) {
	int len = strlen(filename);
	if (len > 4 && filename[len - 4] == '.') {
		string ret = (string)malloc((len - 3) * sizeof(char));
		strncpy(ret, filename, len - 4);
		ret[len - 4] = '\0';
		return ret;
	} else {
		return String(filename);
	}
}

// 删除孤儿结点
void delete_nofa_node(G_nodeList bg) {
	while (TRUE) {
		bool nice = TRUE;
		G_nodeList temp_bg = bg->tail, prev = bg;
		while (temp_bg) {
			G_node n = temp_bg->head;
			if (G_pred(n) == NULL) {
				nice = FALSE;
				G_nodeList succ = G_succ(n);
				while (succ) {
					G_rmEdge(n, succ->head);
					succ = succ->tail;
				}
				prev->tail = temp_bg->tail;
			} else {
				prev = prev->tail;
			}
			temp_bg = temp_bg->tail;
		}
		if (nice) {
			break;
		}
	}
}

// 重新建图
AS_blockList bg2AS_BlockList(G_nodeList bg) {
	AS_blockList asbl = NULL, tail = NULL;
	while (bg) {
		AS_block asb = G_nodeInfo(bg->head);
		if (tail == NULL) {
			asbl = tail = AS_BlockList(asb, NULL);
		} else {
			tail->tail = AS_BlockList(asb, NULL);
			tail = tail->tail;
		}
		bg = bg->tail;
	}
	return asbl;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: cc filename");
	}

	// 输入输出重定向
	string filename = stripFilename(argv[1]);

	// 标准输入用作parse
	freopen(String_format("%s.fmj", filename), "r", stdin);
	FILE* AST = fopen(String_format("%s.ast", filename), "w");
	FILE* IRP = fopen(String_format("%s.irp", filename), "w");
	FILE* STM = fopen(String_format("%s.stm", filename), "w");
	FILE* LIV = fopen(String_format("%s.liv", filename), "w");

#ifdef LLVM
	FILE* LL = fopen(String_format("%s.ll", filename), "w");
#else
	FILE* RPI = fopen(String_format("%s.s", filename), "w");
#endif

    yyparse(); 
	// 输出AST
    printA_Prog(AST, root);
    
	// 类型检查
	check_Prog(stderr, root);
    
	// 翻译成IR+
	T_funcDeclList fl = translate_Prog(stderr, root);
	// 输出IR+
	printFuncDeclList_readable(IRP, fl);

#ifdef LLVM
#else
	// 输出RPI需要的头
	print_head(RPI);
#endif

	T_stm s;
    while (fl) {
		s=fl->head->stm;
		// 线性化
		T_stmList sl = C_linearize(s);
		// 输出线性的IR+
		fprintf(STM, "\nLinearized IR Tree:\n");
		printStmList_linearized(STM, sl, 0);
		fprintf(STM, "\n");

		// 转换为基本块
		struct C_block c = C_basicBlocks(sl);

		// 指令选择
		AS_instrList prolog = NULL, epilog = NULL;
		AS_blockList asbl = NULL, tail = NULL;
		prolog = progen(fl->head);
		//fprintf(STM, "How It's Broken Up:\n");
		for (C_stmListList sList=c.stmLists; sList; sList=sList->tail) {
			fprintf(STM, "\nFor Label=%s\n", S_name(sList->head->head->u.LABEL));
			printStmList_linearized(STM, sList->head, 0);
			AS_block asb = AS_Block(codegen(sList->head));
			if (tail == NULL) {
				asbl = tail = AS_BlockList(asb, NULL);
			} else {
				tail->tail = AS_BlockList(asb, NULL);
				tail = tail->tail;
			}
		}
		epilog = epigen(c.label);

#ifdef LLVM
		// 插入头结点
		Temp_label root = Temp_newlabel();
		AS_instr ri1 = AS_Label(String_format("%s:", Temp_labelstring(root)), root);
		Temp_label temp_l = NULL;
		if (asbl == NULL) {
			temp_l = c.label;
		}
		else {
			temp_l = asbl->head->label;
		}
		AS_instr ri2 = AS_Oper(String_format("br label %%`j0"), fl->head->args, NULL, AS_Targets(Temp_LabelList(temp_l, NULL)));
		AS_block root_block = AS_Block(AS_InstrList(ri1, AS_InstrList(ri2, NULL)));
		asbl = AS_BlockList(root_block, asbl);
		// 输出
		G_nodeList bg = Create_bg(asbl);
		//删除孤儿结点后重新建图
		delete_nofa_node(bg);
		AS_blockList reasbl = bg2AS_BlockList(bg);
		G_nodeList rebg = Create_bg(reasbl);
		// SSA
		ssa_form(rebg);
		AS_instrList il = AS_traceSchedule(reasbl, prolog, epilog, FALSE);
		AS_printInstrList(LL, il, Temp_name());

		// 数据流图
		G_graph G = FG_AssemFlowGraph(il);
		G_show(LIV, G_nodes(G), (void*)show);
		// 活跃分析
		G_nodeList lg = Liveness(G_nodes(G));
		Show_Liveness(LIV, lg);
		// 冲突图
		fprintf(LIV, "------Interference Graph---------\n");
		G_nodeList ig = Create_ig(lg);
		// Show_ig(LIV, ig);
#else
		
		AS_instrList il = AS_traceSchedule(asbl, prolog, epilog, FALSE);
		// 数据流图
		G_graph G = FG_AssemFlowGraph(il);
		G_show(LIV, G_nodes(G), (void*)show);
		// 活跃分析
		G_nodeList lg = Liveness(G_nodes(G));
		Show_Liveness(LIV, lg);
		// 冲突图
		fprintf(LIV, "------Interference Graph---------\n");
		G_nodeList ig = Create_ig(lg);
		// Show_ig(LIV, ig);
		// 寄存器分配
		Temp_map coloring = RA_regAlloc(ig, il);
		AS_printInstrList(RPI, il, coloring);
#endif 

		// 输出canonical Tree
		fprintf(STM, "\n\nThe Final Canonical Tree:\n");
		printStmList_linearized(STM, C_traceSchedule(c), 0);

		fl = fl->tail;
    }

#ifdef LLVM
	print_declare(LL);
#else
	// 输出RPI需要的尾
	print_tail(RPI);
#endif

    return 0;
}