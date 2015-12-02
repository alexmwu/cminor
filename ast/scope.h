#ifndef SCOPE_H
#define SCOPE_H

#include "symbol.h"
#include "hash_table.h"

struct scope_list {
  struct hash_table *table;
  struct scope_list *next;
  struct scope_list *prev;
};

struct scope_list *curr_scope;
int resolve_error_count;

void scope_enter();
void scope_exit();
void scope_bind(const char *name, struct symbol *s);
// both lookups return null if a symbol is not
// found; need to handle
struct symbol *scope_curr_lookup(const char *name);
struct symbol *scope_lookup(const char *name);
void scope_print_all();

#endif

