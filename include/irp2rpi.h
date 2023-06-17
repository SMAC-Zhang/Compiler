#ifndef __IRP2RPI_H
#define __IRP2RPI_H

#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "treep.h"
#include "assem.h"
#include "canon.h"
#include "assemblock.h"

#define IL(a, b) AS_InstrList(a, b)
#define MI(a, b, c) AS_Move(a, b, c)
#define LI(a, b) AS_Label(a, b)
#define OI(a, b, c, d) AS_Oper(a, b, c, d)
#define T(a) Temp_TempList(a, NULL)
#define TL(a, b) Temp_TempList(a, b)
#define LL(a, b) Temp_LabelList(a, b)
#define L(a) Temp_LabelList(a, NULL)
#define Targets(a) AS_Targets(a)

struct AS_prog {AS_instrList prolog, epilog; AS_blockList bl;};

struct AS_prog irp2rpi(struct C_block, T_funcDecl);

#endif