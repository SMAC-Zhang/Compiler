/*
 * types.c - 
 *
 * All types and functions declared in this header file begin with "Ty_"
 * Linked list types end with "..list"
 */

#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "types.h"


static struct Ty_ty_ tyint = {Ty_int, FALSE, FALSE};
Ty_ty Ty_Int(void) {return &tyint;}

static struct Ty_ty_ tyarray = {Ty_array, FALSE, FALSE};
Ty_ty Ty_Array(void) {return &tyarray;}

static struct Ty_ty_ tyloctionint = {Ty_int, TRUE, FALSE};
Ty_ty Ty_LocationInt(void) {return &tyloctionint;}

static struct Ty_ty_ tyPointerArray = {Ty_array, FALSE, TRUE};
Ty_ty Ty_PointerArray(void) {return &tyPointerArray;}

static struct Ty_ty_ tyPointerLocationArray = {Ty_array, TRUE, TRUE};
Ty_ty Ty_LocationPointerArray(void) {return &tyPointerLocationArray;} 

Ty_tyList Ty_TyList(Ty_ty head, Ty_tyList tail)
{Ty_tyList p = checked_malloc(sizeof(*p));
 p->head=head;
 p->tail=tail;
 return p;
}

Ty_field Ty_Field(S_symbol name, Ty_ty ty)
{Ty_field p = checked_malloc(sizeof(*p));
 p->name=name;
 p->ty=ty;
 return p;
}

Ty_fieldList Ty_FieldList(Ty_field head, Ty_fieldList tail)
{Ty_fieldList p = checked_malloc(sizeof(*p));
 p->head=head;
 p->tail=tail;
 return p;
}


