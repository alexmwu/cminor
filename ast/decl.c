#include "decl.h"
#include <stdlib.h>
#include <stdio.h>

struct decl *decl_create(struct expr *name, struct type *t, struct expr *v, struct stmt *c, struct decl *next) {
  struct decl *d = malloc(sizeof *d);
  d -> name = name;
  d -> type = t;
  d -> value = v;
  d -> code = c;
  d -> next = next;
  d -> symbol = 0;
  return d;
}

void decl_print(struct decl *d, int indent) {
  if(!d) return;
  print_indent(indent);
  expr_print(d -> name);
  printf(": ");
  type_print(d -> type);
  if(d -> value) {
    printf(" = ");
    // expr_list rather than just one expr
    if(d -> value -> next) {
      printf("{");
      expr_print(d -> value);
      printf("}");
    }
    else {
      expr_print(d -> value);
    }
    printf(";\n");
  }
  else if(d -> code) {
    printf(" = ");
    printf("{\n");
    stmt_print(d -> code, indent + 1);
    printf("}\n");
  }
  else {
    printf(";\n");
  }
  decl_print(d -> next, indent);
}

void decl_free(struct decl *d) {
  if(!d) return;
  expr_free(d -> name);
  type_free(d -> type);
  expr_free(d -> value);
  stmt_free(d -> code);
  symbol_free(d -> symbol);
  decl_free(d -> next);
  free(d);
}

void decl_resolve(struct decl *d, symbol_t kind) {
  if(!d) return;
  struct symbol *sym = symbol_create(kind, d -> type, d -> name);
  if(scope_lookup(d -> name -> name)) {
    fprintf(
  }
  scope_bind(d -> name -> name, sym);
  decl_resolve(d -> next, kind);
}

