#ifndef __INTERP_H
#define __INTERP_H

#include <stdio.h>
#include "slp.h"
#include "table.h"

void interp_stmt(FILE *stream, A_stm stm);
#endif // #ifndef __INTERP_H