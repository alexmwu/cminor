#include "param_list.h"
#include <stdlib.h>

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

void param_list_fprint(FILE *f, struct param_list *a) {
  if(!a) return;
  expr_fprint(f, a -> name);
  fprintf(f, ": ");
  type_fprint(f, a -> type);
  if(a -> next) {
    fprintf(f, ", ");
    param_list_fprint(f, a -> next);
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
int param_list_resolve(struct param_list *a, int which) {
  // want to return number of params
  if(!a) return which - 1;
  struct symbol *new = symbol_create(SYMBOL_PARAM, a -> type, a -> name);
  new -> which = which;
  a -> symbol = new;
  scope_bind(a -> name -> name, new);
  int ret = param_list_resolve(a -> next, which + 1);
  return ret;
}

void param_list_typecheck(struct param_list *p_list, struct expr *exp_list, const char *name) {
  struct param_list *curr_param = p_list;
  struct expr *curr_exp = exp_list;
  struct type *curr_type = 0;
  while(curr_param && curr_exp) {
    // TODO: somehow this causes a memory leak
    if(curr_type) type_delete(curr_type);
    curr_type = expr_typecheck(curr_exp);
    if(!type_compare(curr_param -> type, curr_type)) {
        fprintf(stderr, "TYPE_ERROR: parameter type (");
        type_fprint(stderr, curr_param -> type);
        fprintf(stderr, ") for function %s does not match argument (", name);
        type_fprint(stderr, curr_type);
        fprintf(stderr, ")\n");
        type_error_count++;
        type_delete(curr_type);
        return;
    }
    curr_param = curr_param -> next;
    curr_exp = curr_exp -> next;
  }
  // if there is still a param or expression
  if(curr_param) {
    fprintf(stderr, "TYPE_ERROR: there are more parameters (");
    param_list_fprint(stderr, p_list);
    fprintf(stderr, ") than arguments");
    expr_typecheck_err_print(stderr, exp_list);
    type_error_count++;
  }
  else if(curr_exp) {
    fprintf(stderr, "TYPE_ERROR: there are more arguments (");
    expr_fprint(stderr, exp_list);
    fprintf(stderr, ") than parameters (");
    param_list_fprint(stderr, p_list);
    fprintf(stderr, ")\n");
    type_error_count++;
  }
  type_delete(curr_type);
}

