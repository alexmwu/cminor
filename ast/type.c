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

void type_free(struct type *t) {
  if(!t) return;
  param_list_free(t -> params);
  type_free(t -> subtype);
  expr_free(t -> expr);
  free(t);
}

// will cause issues upon freeing
struct type *type_copy(struct type *t) {
  struct type *new = malloc(sizeof *new);
  new -> kind = t -> kind;
  new -> params = t -> params;
  new -> subtype = t -> subtype;
  new -> expr = t -> expr;
  return new;
}

int type_compare(struct type *a, struct type *b) {
  if(a -> kind == b -> kind) {
    switch(a -> kind) {
      // should never be called on non-default cases
      // but this handles it if that happens
      case TYPE_ARRAY:
        fprintf(stderr, "Bad function call on type_compare with type_kind TYPE_ARRAY\n");
        exit(1);
        break;
      case TYPE_ARRAY_DECL:
        fprintf(stderr, "Bad function call on type_compare with type_kind TYPE_ARRAY_DECL\n");
        exit(1);
        break;
      case TYPE_FUNCTION:
        fprintf(stderr, "Bad function call on type_compare with type_kind TYPE_FUNCTION\n");
        exit(1);
        break;
      default:
        return 1;
        break;
    }
  }
  return 0;
}


// should only be used on type pointers not
// contained in decls
void type_delete(struct type *t) {
  if(!t) return;
  free(t);
}
