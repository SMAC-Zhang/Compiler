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

A_prog root;
A_prog prog1();

int main(int argc, char* argv[]) {
    bool friendly = FALSE;

	if (argc != 5) {
		fprintf(stderr, "Usage: friendly testxx.irp testxx.line testxx.llir\n");
		exit(1);
	}

    if (argc > 1 && strcmp(argv[1], "friendly")==0) {
       friendly = TRUE;
	}

    yyparse(); 
    // printA_Prog(stdout, root);
    FILE* irp = fopen(argv[2], "w");
	FILE* line = fopen(argv[3], "w");
	FILE* llir = fopen(argv[4], "w");
    check_Prog(irp, root);
    T_funcDeclList fl = translate_Prog(irp, root);
    T_stm s;
    
    while (fl) {
		fprintf(line, "Original IR Tree:\n");
		s=fl->head->stm;
		if (friendly) {
			printFuncDeclList_readable(line, T_FuncDeclList(fl->head, NULL));
		} else {
			printFuncDeclList(line, T_FuncDeclList(fl->head, NULL));
		}
		fprintf(llir, "define i64 @%s(%s) {\n", fl->head->name, args_to_string(fl->head->args));
		T_stmList sl = C_linearize(s);
		
		fprintf(line, "\nLinearized IR Tree:\n");
		printStmList_linearized(line, sl, 0);
		fprintf(line, "\n");
		
		struct C_block c = C_basicBlocks(sl);

		fprintf(line, "How It's Broken Up:\n");
		for (C_stmListList sList=c.stmLists; sList; sList=sList->tail) {
			fprintf(line, "\nFor Label=%s\n", S_name(sList->head->head->u.LABEL));
			printStmList_linearized(line, sList->head, 0);
			AS_instrList al = codegen(sList->head);
			AS_printInstrList(llir, al, Temp_name());
		}
		fprintf(llir, "}\n");

		fprintf(line, "\n\nThe Final Canonical Tree:\n");
		printStmList_linearized(line, C_traceSchedule(c), 0);

		fl = fl->tail;
    }

    fprintf(llir, "declare ptr @malloc(i64)\n");
    return 0;
}