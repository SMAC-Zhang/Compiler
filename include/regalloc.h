#ifndef __REGALLOC_H
#define __REGALLOC_H

#include "temp.h"
#include "graph.h"
#include "assem.h"

Temp_map RA_regAlloc(G_nodeList ig,  AS_instrList il);

#endif