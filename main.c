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
#include "codegen.h"
#include "assem.h"
#include "assemblock.h"
#include "graph.h"
#include "bg.h"
#include "liveness.h"
#include "flowgraph.h"
#include "ig.h"
#include "ssa.h"

A_prog root;
A_prog prog1();

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

int main(int argc, char* argv[]) {

    yyparse(); 
    // printA_Prog(stdout, root);
    check_Prog(stderr, root);
    T_funcDeclList fl = translate_Prog(stderr, root);
    
	T_stm s;
    while (fl) {
		s=fl->head->stm;
		T_stmList sl = C_linearize(s);
				
		struct C_block c = C_basicBlocks(sl);
		AS_instrList prolog = NULL, body = NULL, epilog = NULL;
		AS_blockList asbl = NULL, tail = NULL;
		prolog = progen(fl->head);
		
		for (C_stmListList sList=c.stmLists; sList; sList=sList->tail) {
			AS_block asb = AS_Block(codegen(sList->head));
			if (tail == NULL) {
				asbl = tail = AS_BlockList(asb, NULL);
			} else {
				tail->tail = AS_BlockList(asb, NULL);
				tail = tail->tail;
			}
		}
		epilog = epigen(c.label);

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
		//delete_nofa_node(bg);

		ssa_form(bg);
		AS_instrList il = AS_traceSchedule(asbl, prolog, epilog, FALSE);
		AS_printInstrList(stdout, il, Temp_name());
		
		fl = fl->tail;
    }

    printf("declare ptr @malloc(i64)\n");
    return 0;
}