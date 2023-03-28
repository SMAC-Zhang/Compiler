#ifndef __TYPES_H
#define __TYPES_H

#include "symbol.h"
#include "util.h"
/*
 * types.h - 
 *
 * All types and functions declared in this header file begin with "Ty_"
 * Linked list types end with "..list"
 */

typedef struct Ty_ty_ *Ty_ty;
typedef struct Ty_tyList_ *Ty_tyList;
typedef struct Ty_field_ *Ty_field;
typedef struct Ty_fieldList_ *Ty_fieldList;

struct Ty_ty_ { enum {Ty_int, Ty_array, Ty_name} kind;
				bool location;	
				bool pointer;
			  };

struct Ty_tyList_ {Ty_ty head; Ty_tyList tail;};
struct Ty_field_ {S_symbol name; Ty_ty ty;};
struct Ty_fieldList_ {Ty_field head; Ty_fieldList tail;};

Ty_ty Ty_Int(void);
Ty_ty Ty_Array(void);
Ty_ty Ty_LocationInt(void);
Ty_ty Ty_PointerArray(void);
Ty_ty Ty_LocationPointerArray(void);


Ty_tyList Ty_TyList(Ty_ty head, Ty_tyList tail);
Ty_field Ty_Field(S_symbol name, Ty_ty ty);
Ty_fieldList Ty_FieldList(Ty_field head, Ty_fieldList tail);

#endif