#include "expr.h"
#include <stdlib.h>

struct expr *expr_create(expr_t kind, struct expr *left, struct expr* right) {
  struct expr *e = malloc(sizeof *e);
  e -> kind = kind;
  e -> left = left;
  e -> right = right;
  return e;
}

struct expr* expr_create_name(const char *n) {
  struct expr *e = malloc(sizeof *e);
  e -> kind = EXPR_IDENT;

}