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
      printf("{\n");
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
    printf("{");
    stmt_print(d -> code, indent + 1);
    printf("}\n");
  }
  else {
    printf(";\n");
  }
  decl_print(d -> next, indent);
}
