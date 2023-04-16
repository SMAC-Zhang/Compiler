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

Ty_ty Ty_Class(string id) {
    Ty_ty ty = checked_malloc(sizeof(struct Ty_ty_));
    ty->kind = Ty_class;
    ty->location = TRUE;
    ty->pointer = TRUE;
    ty->id = id;
}

Ty_tyList Ty_TyList(Ty_ty head, Ty_tyList tail) {
    Ty_tyList tl = checked_malloc(sizeof(* tl));
    tl->head = head;
    tl->tail = tail;
    return tl;
}
