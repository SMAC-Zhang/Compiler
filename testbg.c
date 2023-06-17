#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "assem.h"
#include "graph.h"
#include "assemblock.h"
#include "bg.h"

#define IL(a, b) AS_InstrList(a, b)
#define MI(a, b, c) AS_Move(a, b, c)
#define LI(a, b) AS_Label(a, b)
#define OI(a, b, c, d) AS_Oper(a, b, c, d)
#define T(a) Temp_TempList(a, NULL)
#define TL(a, b) Temp_TempList(a, b)
#define LL(a, b) Temp_LabelList(a, b)
#define L(a) Temp_LabelList(a, NULL)
#define Targets(a) AS_Targets(a)

static AS_instrList prolog, epilog;
AS_blockList blist1(void) {

#include "../bgtests/test03.as"

}

int main(void) {
    AS_blockList bl=blist1();

    G_nodeList bg=Create_bg(bl);

    printf("------Basic Block Graph---------\n");

    Show_bg(stdout, bg);
    AS_instrList il = AS_traceSchedule(bl, prolog, epilog, FALSE);

    printf("------~Final traced StmList---------\n");
    AS_printInstrList(stdout, il, Temp_name());
    return 0;
}
