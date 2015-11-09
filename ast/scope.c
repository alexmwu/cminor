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

void remove_scopes(struct hash_table *ht) {
  char **key;
  struct symbol *value;
  hash_table_firstkey(ht);
  while(hash_table_nextkey(ht, key, (void **) &value)){
    free(value);
  }
}

void scope_exit() {
  remove_scopes(curr_scope -> table);
  hash_table_clear(curr_scope -> table);
  hash_table_delete(curr_scope -> table);
  struct scope_list *exited_scope = curr_scope;
  curr_scope = curr_scope -> prev;
  free(exited_scope);
}

void scope_bind(const char *name, struct symbol *s) {
  int outCode = hash_table_insert(curr_scope -> table, name, s);
  if(!outCode) {
    fprintf(stderr, "Error in adding a key to the hash table\n");
    exit(1);
  }
}

struct symbol *scope_lookup(const char *name) {
  return hash_table_lookup(curr_scope -> table, name);
}

