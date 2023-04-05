#ifndef __TYPES_H
#define __TYPES_H

#include "symbol.h"
#include "util.h"
#include "fdmjast.h"
/*
 * types.h - 
 *
 * All types and functions declared in this header file begin with "Ty_"
 * Linked list types end with "..list"
 */

typedef struct Ty_ty_ *Ty_ty;
typedef struct Ty_tyList_ *Ty_tyList;

struct Ty_ty_ { enum {Ty_int, Ty_array, Ty_class, Ty_method} kind;
				bool location;	
				bool pointer;
			  	string id;
				Ty_tyList fl;
				Ty_ty ret;
			  };

struct Ty_tyList_ {Ty_ty head; Ty_tyList tail;};

Ty_ty Ty_Int(void);
Ty_ty Ty_Array(void);
Ty_ty Ty_LocationInt(void);
Ty_ty Ty_PointerArray(void);
Ty_ty Ty_LocationPointerArray(void);
Ty_ty Ty_Class(string id);
Ty_ty Ty_Method(string id, Ty_tyList fl, Ty_ty ret);

#endif