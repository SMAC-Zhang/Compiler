#include <string.h>
#include <stdio.h>

#include "table.h"

Table_ head;

Table_ Table(string id, int value, Table_ tail)  { 
    Table_ t = checked_malloc(sizeof *t); 
    t->id = String(id);
    t->value = value;
    t->tail = tail; 
    return t;
}

int find_in_table(string id) {
    Table_ t = head;
    while (t) {
        if (strcmp(t->id, id) == 0) {
            return t->value;
        }
        t = t->tail;
    }
    fprintf(stderr, "find_in_table_error! id:%s\n", id);
    return 0;
}
