#include "stmt.h"
#include "library.h"
#include <stdlib.h>
#include <stdio.h>

struct stmt *stmt_create(stmt_kind_t kind, struct decl *d, struct expr *init_expr, struct expr *e, struct expr *next_expr, struct stmt *body, struct stmt *else_body) {
  struct stmt *s = malloc(sizeof *s);
  s -> kind = kind;
  s -> decl = d;
  s -> init_expr = init_expr;
  s -> expr = e;
  s -> next_expr = next_expr;
  s -> body = body;
  s -> else_body = else_body;
  return s;
}

void stmt_print_body(struct stmt *s, int indent) {
  if(s -> kind == STMT_BLOCK) {
    /*pretty print a space if the next stmt is a block*/
    printf(" ");
    stmt_print(s -> body, indent);
  }
  else {
    /*otherwise, new line*/
    printf("\n");
    stmt_print(s -> body, indent + 1);
  }
}

void stmt_print(struct stmt *s, int indent) {
  if(!s) return;
  print_indent(indent);
  switch(s -> kind) {
    case STMT_DECL:
      decl_print(s -> decl, indent);
      printf("\n");
      break;
    case STMT_EXPR:
      expr_print(s -> expr);
      printf(";\n");
      break;
    case STMT_IF_ELSE:
      printf("if(");
      expr_print(s -> expr);
      printf(")");
      stmt_print_body(s -> body, indent);
      if(s -> else_body) {
        print_indent(indent);
        printf("else");
        stmt_print_body(s -> else_body, indent);
      }
      break;
    case STMT_FOR:
      printf("for(");
      expr_print(s -> init_expr);
      printf("; ");
      expr_print(s -> expr);
      printf("; ");
      expr_print(s -> next_expr);
      printf(")");
      stmt_print_body(s -> body, indent);
      break;
    case STMT_WHILE:
      /*
       *while loops are not in the CMinor spec so this should not get called.
       *if it does, it breaks immediately
       */
      break;
    case STMT_PRINT:
      printf("print");
      if(s -> expr) {
        printf(" ");
        expr_print(s -> expr);
      }
      printf(";\n");
      break;
    case STMT_RET:
      printf("return");
      if(s -> expr) {
        printf(" ");
        expr_print(s -> expr);
      }
      printf(";\n");
      break;
    case STMT_BLOCK:
      printf("{\n");
      stmt_print(s -> body, indent + 1);
      printf("}\n");
      break;
  }
  stmt_print(s -> next, indent);
}

