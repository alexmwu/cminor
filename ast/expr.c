#include "expr.h"
#include "library.h"
#include <stdlib.h>
#include <stdio.h>

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

void expr_print(struct expr *e) {
  if(!e) return;
  switch(e -> kind) {
    case EXPR_PLUS:
      expr_print(e -> left);
      printf(" + ");
      expr_print(e -> right);
      break;
    case EXPR_MIN:
      if(!e -> left) {
        printf("-");
        expr_print(e -> right);
        break;
      }
      expr_print(e -> left);
      printf(" - ");
      expr_print(e -> right);
      break;
    case EXPR_MUL:
      expr_print(e -> left);
      printf(" * ");
      expr_print(e -> right);
      break;
    case EXPR_DIV:
      expr_print(e -> left);
      printf(" / ");
      expr_print(e -> right);
      break;
    case EXPR_MOD:
      expr_print(e -> left);
      printf(" %% ");
      expr_print(e -> right);
      break;
    case EXPR_PLUSPLUS:
      if(e -> left) {
        expr_print(e -> left);
        printf("++");
      }
      else if(e -> right) {
        printf("++");
        expr_print(e -> right);
      }
      else {
        fprintf(stderr, "Pre/Post error: 0 or 2 identifiers\n");
        exit(1);
      }
      break;
    case EXPR_MINMIN:
      if(e -> left) {
        expr_print(e -> left);
        printf("--");
      }
      else if(e -> right) {
        printf("--");
        expr_print(e -> right);
      }
      else {
        fprintf(stderr, "Pre/Post error: 0 or 2 identifiers\n");
        exit(1);
      }
      break;
    case EXPR_EXP:
      expr_print(e -> left);
      printf(" ^ ");
      expr_print(e -> right);
      break;
    case EXPR_LT:
      expr_print(e -> left);
      printf(" < ");
      expr_print(e -> right);
      break;
    case EXPR_LE:
      expr_print(e -> left);
      printf(" <= ");
      expr_print(e -> right);
      break;
    case EXPR_GT:
      expr_print(e -> left);
      printf(" > ");
      expr_print(e -> right);
      break;
    case EXPR_GE:
      expr_print(e -> left);
      printf(" >= ");
      expr_print(e -> right);
      break;
    case EXPR_EQEQ:
      expr_print(e -> left);
      printf(" == ");
      expr_print(e -> right);
      break;
    case EXPR_NE:
      expr_print(e -> left);
      printf(" != ");
      expr_print(e -> right);
      break;
    case EXPR_AND:
      expr_print(e -> left);
      printf(" && ");
      expr_print(e -> right);
      break;
    case EXPR_OR:
      expr_print(e -> left);
      printf(" || ");
      expr_print(e -> right);
      break;
    case EXPR_NOT:
      printf("!");
      if(!e -> right) {
        fprintf(stderr, "NOT Error: no right expression\n");
        exit(1);
      }
      expr_print(e -> right);
      break;
    case EXPR_ARREQ:
      /*
       *EXPR_ARREQ has left of ident, right of expr (for index),
       *and next of expr (what it is set to).
       */
      expr_print(e -> left);
      printf("[");
      expr_print(e -> right);
      printf("]");
      printf(" = ");
      expr_print(e -> next);
      break;
    case EXPR_EQ:
      expr_print(e -> left);
      printf(" = ");
      expr_print(e -> right);
      break;
    case EXPR_ARR:
      expr_print(e -> left);
      printf("[");
      expr_print(e -> right);
      printf("]");
      break;
    case EXPR_GROUP:
      printf("(");
      /*
       *Grouping is defined as one expression around paren
       *(so just print e->next which should be just one expr)
       */
      expr_print(e -> next);
      printf(")");
      break;
    case EXPR_FUNC:
      expr_print(e -> left);
      printf("(");
      expr_print(e -> next);
      printf(")");
      break;
    case EXPR_LIST:
      /*
       *Expr list defined as linked list of exprs.
       *each expr is held in e -> left
       */
      expr_print(e -> left);
      if(e -> next) {
        printf(", ");
        expr_print(e -> next);
      }
      break;
    case EXPR_TRUE:
      print_boolean(1);
      break;
    case EXPR_FALSE:
      print_boolean(0);
      break;
    case EXPR_INTLIT:
      print_integer(e -> literal_value);
      break;
    case EXPR_CHARLIT:
      print_character(e -> char_literal);
      break;
    case EXPR_STRLIT:
      printf("\"");
      print_string(e -> string_literal);
      printf("\"");
      break;
    case EXPR_IDENT:
      print_string(e -> name);
      break;
  }
}