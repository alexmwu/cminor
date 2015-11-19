#include "param_list.h"
#include <stdlib.h>
#include <stdio.h>

struct param_list *param_list_create(struct expr *name, struct type *type, struct param_list *next) {
  struct param_list *pl = malloc(sizeof *pl);
  pl -> name = name;
  pl -> type = type;
  pl -> next = next;
  pl -> symbol = 0;
  return pl;
}

void param_list_print(struct param_list *a) {
  if(!a) return;
  expr_print(a -> name);
  printf(": ");
  type_print(a -> type);
  if(a -> next) {
    printf(", ");
    param_list_print(a -> next);
  }
}

void param_list_free(struct param_list *a) {
  if(!a) return;
  expr_free(a -> name);
  type_free(a -> type);
  symbol_free(a -> symbol);
  param_list_free(a -> next);
  free(a);
}

// pass in current scope, but should let the
// decl handle this (rather than use global)
void param_list_resolve(struct param_list *a, int which) {
  if(!a) return;
  struct symbol *new = symbol_create(SYMBOL_PARAM, a -> type, a -> name);
  new -> which = which;
  scope_bind(a -> name -> name, new);
  param_list_resolve(a -> next, which + 1);
}


// TODO: finish this
int param_list_typecheck(struct param_list *p_list, struct expr *exp_list) {
  struct param_list *curr_param;
  struct expr *curr_exp;
  while(curr_param && curr_exp) {
    if(!type_compare(curr_param -> type)) return 0;
  }
}

