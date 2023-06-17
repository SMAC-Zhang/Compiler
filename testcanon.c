#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "treep.h"
#include "canon.h"
#include "printtreep.h"
#include "pr_tree_readable.h"
#include "pr_linearized.h"

int main(int argc, char *argv[]) {
    bool friendly=FALSE;

    if (argc>1 && strcmp(argv[1], "friendly")==0) 
       friendly=TRUE;
  

    T_funcDeclList fl=tree1(argv[2]);
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
