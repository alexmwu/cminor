#include "type.h"
#include <stdlib.h>
#include <stdio.h>

struct type *type_create(type_kind_t kind, struct param_list *params, struct type *subtype, struct expr *expr) {
  struct type *t = malloc(sizeof *t);
  t -> kind = kind;
  t -> params = params;
  t -> subtype = subtype;
  t -> expr = expr;
  return t;
}

void type_print(struct type *t) {
  if(!t) return;
  switch(t -> kind) {
    case TYPE_BOOLEAN:
      printf("boolean");
      break;
    case TYPE_CHARACTER:
      printf("char");
      break;
    case TYPE_INTEGER:
      printf("integer");
      break;
    case TYPE_STRING:
      printf("string");
      break;
    case TYPE_ARRAY:
      printf("array [] ");
      type_print(t -> subtype);
      break;
    case TYPE_ARRAY_DECL:
      printf("array ");
      printf("[");
      expr_print(t -> expr);
      printf("] ");
      type_print(t -> subtype);
      break;
    case TYPE_FUNCTION:
      printf("function ");
      type_print(t -> subtype);
      printf(" (");
      param_list_print(t -> params);
      printf(")");
      break;
    case TYPE_VOID:
      printf("void");
      break;
  }
}
