#ifndef __CODEGEN_H
#define __CODEGEN_H

#include "assem.h"
#include "treep.h"

AS_instrList codegen(T_stmList stmList);
AS_instrList progen(T_funcDecl fl);
AS_instrList epigen(Temp_label tl);

#endif