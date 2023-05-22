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
//#include "codegen.h"
#include "irp2rpi.h"
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

void rpi_show(AS_instr ins) {
    FG_Showinfo(stdout, ins, Temp_name());
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
		
		struct AS_prog p = irp2rpi(c, fl->head);
		// 输出
		G_nodeList bg = Create_bg(p.bl);

		printf("------Basic Block Graph---------\n");

		Show_bg(stdout, bg);
		AS_instrList il = AS_traceSchedule(p.bl, p.prolog, p.epilog, TRUE);

		printf("------Final traced StmList---------\n");
		AS_printInstrList(stdout, il, Temp_name());

		printf("------Instruction level flow graph---------\n");
		G_graph G=FG_AssemFlowGraph(il);
		G_show(stdout, G_nodes(G), (void*)rpi_show);

		printf("\n\n------Liveness result---------\n");
		G_nodeList lg = Liveness(G_nodes(G));
		Show_Liveness(stdout, lg);
		printf("\n\n------Interference Graph---------\n");
		G_nodeList ig = Create_ig(lg);
		Show_ig(stdout, ig);
		
		fl = fl->tail;
    }

    return 0;
}