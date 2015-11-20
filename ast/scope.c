#include "scope.h"
#include <stdlib.h>
#include <stdio.h>

void scope_enter() {
  if(!curr_scope) {
    curr_scope = malloc(sizeof *curr_scope);
    // default bucket size, hash function
    curr_scope -> table = hash_table_create(0, 0);
    curr_scope -> prev = 0;
    curr_scope -> next = 0;
    return;
  }

  struct scope_list *new_scope = malloc(sizeof *new_scope);
  curr_scope -> next = new_scope;
  new_scope -> prev = curr_scope;
  new_scope -> table = hash_table_create(0, 0);
  new_scope -> next = 0;
  curr_scope = new_scope;
}

void scope_print_table(struct hash_table *ht, char **key, struct symbol *value) {
  hash_table_firstkey(ht);
  while(hash_table_nextkey(ht, key, (void **) &value)) {
    symbol_print(value);
  }
}

void scope_print_all() {
  char **key = malloc(sizeof *key);
  struct symbol *value;
  struct scope_list *ptr = curr_scope;
  while(ptr) {
    printf("New scope:\n");
    scope_print_table(ptr -> table, key, value);
    ptr = ptr -> prev;
  }
  free(key);
}

void scope_exit() {
  hash_table_clear(curr_scope -> table);
  hash_table_delete(curr_scope -> table);
  struct scope_list *exited_scope = curr_scope;
  curr_scope = curr_scope -> prev;
  free(exited_scope);
}

void scope_bind(const char *name, struct symbol *s) {
  int outCode = hash_table_insert(curr_scope -> table, name, s);
  if(!outCode) {
    fprintf(stderr, "RESOLVE_ERROR: redeclaration of %s.\n", name);
    resolve_error_count++;
  }
}

struct symbol *scope_lookup(const char *name) {
  struct symbol *tmp;
  // pointer to current table to look at
  struct scope_list *ptr = curr_scope;
  // keep doing while can't find the key and
  // the symbol table ptr is not null
  do {
    tmp = hash_table_lookup(ptr -> table, name);
    ptr = ptr -> prev;
  } while(!tmp && ptr);
  return tmp;
}

