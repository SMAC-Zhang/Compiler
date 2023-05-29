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
#include "regalloc.h"

A_prog root;
A_prog prog1();

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

		AS_instrList il = AS_traceSchedule(p.bl, p.prolog, p.epilog, TRUE);
		
		G_graph G = FG_AssemFlowGraph(il);

		G_nodeList lg = Liveness(G_nodes(G));
		// 冲突图
		G_nodeList ig = Create_ig(lg);

		Temp_map coloring = RA_regAlloc(ig, il);

		AS_printInstrList(stdout, il, coloring);

		fl = fl->tail;
    }

    return 0;
}