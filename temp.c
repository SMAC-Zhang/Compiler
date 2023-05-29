/*
 * temp.c - functions to create and manipulate temporary variables which are
 *          used in the IR tree representation before it has been determined
 *          which variables are to go into registers.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "table.h"

#define TEMP_SIZE 4096

struct Temp_temp_ {
  int num;
  int origin;
};

string Temp_labelstring(Temp_label s)
{return S_name(s);
}

static int labels = 0;
static Temp_temp atemp_map[TEMP_SIZE];

Temp_label Temp_newlabel(void)
{char buf[100];
 sprintf(buf,"L%d",labels++);
 return Temp_namedlabel(String(buf));
}

/* The label will be created only if it is not found. */
Temp_label Temp_namedlabel(string s)
{return S_Symbol(s);
}

static int temps = 100;

Temp_temp Temp_newtemp(void)
{Temp_temp p = (Temp_temp) checked_malloc(sizeof (*p));
 p->num=temps++;
 p->origin = p->num - 100;
 {char r[16];
  sprintf(r, "%d", p->num);
  Temp_enter(Temp_name(), p, String(r));
 }
 atemp_map[p->num - 100] = p;
 return p;
}

void Temp_init() { // 初始化r0 - r3, r8 - r10, lr
  for (int i = 0; i < 11; ++i) {
    if (5 <= i && i < 8) {
      continue;
    }
    Temp_temp p = (Temp_temp) checked_malloc(sizeof (*p));
    p->num = i;
    p->origin = i;
    char r[16];
    sprintf(r, "%d", p->num);
    Temp_enter(Temp_name(), p, String(r));
    atemp_map[TEMP_SIZE - 1 - i] = p;
  }
}

Temp_temp get_rtemp(int r) { // 获得r0-r3, lr
  static bool init = FALSE;
  if (!init) {
    Temp_init();
    init = TRUE;
  }
  return atemp_map[TEMP_SIZE - 1 - r];
}

void set_origin(Temp_temp t, int origin) {
  t->origin = origin;
}

int temp_num() {
  return temps - 100;
}

int temp_id(Temp_temp t) {
  if (t->num >= 100) {
    return t->num - 100;
  } else {
    return TEMP_SIZE - 1 - t->num;
  }
}

int temp_id2name(int id) {
  return id + 100;
}

int temp_origin(Temp_temp t) {
  return t->origin;
}

Temp_temp get_temp(int name) {
  return atemp_map[name - 100];
}

struct Temp_map_ {TAB_table tab; Temp_map under;};


Temp_map Temp_name(void) {
 static Temp_map m = NULL;
 if (!m) m=Temp_empty();
 return m;
}

Temp_map newMap(TAB_table tab, Temp_map under) {
  Temp_map m = checked_malloc(sizeof(*m));
  m->tab=tab;
  m->under=under;
  return m;
}

Temp_map Temp_empty(void) {
  return newMap(TAB_empty(), NULL);
}

Temp_map Temp_layerMap(Temp_map over, Temp_map under) {
  if (over==NULL)
      return under;
  else return newMap(over->tab, Temp_layerMap(over->under, under));
}

void Temp_enter(Temp_map m, Temp_temp t, string s) {
  assert(m && m->tab);
  TAB_enter(m->tab,t,s);
}

string Temp_look(Temp_map m, Temp_temp t) {
  string s;
  assert(m && m->tab);
  s = TAB_look(m->tab, t);
  if (s) return s;
  else if (m->under) return Temp_look(m->under, t);
  else return NULL;
}

Temp_tempList Temp_TempList(Temp_temp h, Temp_tempList t) 
{Temp_tempList p = (Temp_tempList) checked_malloc(sizeof (*p));
 p->head=h; p->tail=t;
 return p;
}

Temp_labelList Temp_LabelList(Temp_label h, Temp_labelList t)
{Temp_labelList p = (Temp_labelList) checked_malloc(sizeof (*p));
 p->head=h; p->tail=t;
 return p;
}

static FILE *outfile;
void showit(Temp_temp t, string r) {
  fprintf(outfile, "t%d -> %s\n", t->num, r);
}

void Temp_dumpMap(FILE *out, Temp_map m) {
  outfile=out;
  TAB_dump(m->tab,(void (*)(void *, void*))showit);
  if (m->under) {
     fprintf(out,"---------\n");
     Temp_dumpMap(out,m->under);
  }
}

string args_to_string(Temp_tempList tl) {
  char ret[1024] = {'\0'};
  while (tl) {
    if (tl->tail) {
      strcat(ret, String_format("i64 %t%d, ", tl->head->num));
    } else {
      strcat(ret, String_format("i64 %t%d", tl->head->num));      
    }
    tl = tl->tail;
  }
  return String(ret);
}