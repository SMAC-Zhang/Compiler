#ifndef __TABLE_H
#define __TABLE_H

#include "util.h"

typedef struct table *Table_;
struct table {
    string id;
    int value;
    Table_ tail;
};

typedef struct IntAndTable *IntAndTable_;
struct IntAndTable {
    int I;
    Table_ t;
};

Table_ Table(string id, int value, Table_ tail);
int table_lookup(string id);

#endif // #ifndef __TABLE_H