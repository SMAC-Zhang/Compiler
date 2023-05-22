#include "irp2rpi.h"
#include "graph.h"
#include "bg.h"
#include "flowgraph.h"
#include "liveness.h"
#include "ig.h"

void rpi_show(AS_instr ins) {
    FG_Showinfo(stdout, ins, Temp_name());
}


int main(void) {
    struct C_block b;
    b.stmLists=NULL;
    b.label=NULL;

    struct AS_prog p=irp2rpi(b); // get the AS_blockList (and prolog&epilog)

    G_nodeList bg=Create_bg(p.bl);

    printf("------Basic Block Graph---------\n");

    Show_bg(stdout, bg);
    AS_instrList il = AS_traceSchedule(p.bl, p.prolog, p.epilog, TRUE);

    printf("------Final traced StmList---------\n");
    AS_printInstrList(stdout, il, Temp_name());

    printf("------Instruction level flow graph---------\n");
    G_graph G=FG_AssemFlowGraph(il);
    G_show(stdout, G_nodes(G), (void*)rpi_show);

    printf("\n\n------Liveness result---------\n");
    G_nodeList lg=Liveness(G_nodes(G));
    Show_Liveness(stdout, lg);
    printf("\n\n------Interference Graph---------\n");
    G_nodeList ig=Create_ig(lg);
    Show_ig(stdout, ig);

    return 0;
}
