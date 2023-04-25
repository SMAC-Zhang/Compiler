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

A_prog root;
A_prog prog1();

int main(int argc, char* argv[]) {
    bool friendly=FALSE;

    if (argc>1 && strcmp(argv[1], "friendly")==0) 
       friendly=TRUE;

    yyparse(); 
    // printA_Prog(stdout, root);
    FILE* irp = fopen(argv[2], "w");
    check_Prog(irp, root);
    T_funcDeclList fl = translate_Prog(irp, root);
    T_stm s;
    
    while (fl) {
      fprintf(stdout, "Original IR Tree:\n");
      s=fl->head->stm;
      if (friendly) 
        printFuncDeclList_readable(stdout, T_FuncDeclList(fl->head, NULL));
      else
	printFuncDeclList(stdout, T_FuncDeclList(fl->head, NULL));
      
      T_stmList sl = C_linearize(s);
    
      fprintf(stdout, "\nLinearized IR Tree:\n");
      printStmList_linearized(stdout, sl, 0);
      fprintf(stdout, "\n");
    
      struct C_block c = C_basicBlocks(sl);

      printf("How It's Broken Up:\n");
      for (C_stmListList sList=c.stmLists; sList; sList=sList->tail) {
          fprintf(stdout, "\nFor Label=%s\n", S_name(sList->head->head->u.LABEL));
      printStmList_linearized(stdout, sList->head, 0);
      }

      fprintf(stdout, "\n\nThe Final Canonical Tree:\n");
      printStmList_linearized(stdout, C_traceSchedule(c), 0);
      
      fl=fl->tail;
    }

    fprintf(stdout, "\n");
    return 0;
}