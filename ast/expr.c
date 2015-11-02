#include "expr.h"
#include <stdlib.h>

struct expr *expr_create(expr_t kind, struct expr *left, struct expr *right, struct expr *next) {
  struct expr *e = malloc(sizeof *e);
  e -> kind = kind;
  e -> left = left;
  e -> right = right;
  e -> next = next;
  return e;
}

/*TODO: add symbol (needed in typechecker)*/
struct expr *expr_create_name(const char *n) {
  struct expr *e = expr_create(EXPR_IDENT, 0, 0, 0);
  e -> name = n;
  return e;
}

struct expr *expr_create_boolean_literal(int c) {
  struct expr *e = expr_create(c?EXPR_TRUE:EXPR_FALSE, 0, 0, 0);
  return e;
}

struct expr *expr_create_integer_literal(int c) {
  struct expr *e = expr_create(EXPR_INTLIT, 0, 0, 0);
  return e;
}

struct expr *expr_create_character_literal(char c) {
  struct expr *e = expr_create(EXPR_CHARLIT, 0, 0, 0);
  e -> char_literal = c;
  return e;
}

struct expr *expr_create_string_literal(const char *str) {
  struct expr *e = expr_create(EXPR_STRLIT, 0, 0, 0);
  e -> string_literal = str;
  return e;
}

/*TODO: make print function*/