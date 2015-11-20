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
  s -> next = 0;
  return s;
}

void stmt_print_body(struct stmt *s, int indent) {
  if(s -> kind == STMT_BLOCK) {
    /*pretty print a space if the next stmt is a block*/
    printf(" ");
    stmt_print(s, indent);
  }
  else {
    /*otherwise, new line*/
    printf("\n");
    stmt_print(s, indent + 1);
  }
}

void stmt_print(struct stmt *s, int indent) {
  if(!s) return;
  if(s -> kind != STMT_BLOCK)
    print_indent(indent);

  switch(s -> kind) {
    case STMT_DECL:
      decl_print(s -> decl, indent - 1);
      /*don't print new line because decl always prints new lines*/
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
      print_indent(indent);
      printf("}\n");
      break;
  }
  stmt_print(s -> next, indent);
}

void stmt_free(struct stmt *s) {
  if(!s) return;
  decl_free(s -> decl);
  expr_free(s -> init_expr);
  expr_free(s -> expr);
  expr_free(s -> next_expr);
  stmt_free(s -> body);
  stmt_free(s -> else_body);
  stmt_free(s -> next);
  free(s);
}

void stmt_resolve(struct stmt *s, int which) {
  if(!s) return;
  switch(s -> kind) {
    case STMT_DECL:
      decl_resolve(s -> decl, SYMBOL_LOCAL, which);
      break;
    case STMT_EXPR:
      expr_resolve(s -> expr);
      break;
    case STMT_IF_ELSE:
      expr_resolve(s -> expr);
      stmt_resolve(s -> body, which);
      stmt_resolve(s -> else_body, which);
      break;
    case STMT_FOR:
      expr_resolve(s -> init_expr);
      expr_resolve(s -> expr);
      expr_resolve(s -> next_expr);
      stmt_resolve(s -> body, which);
      break;
    case STMT_WHILE:
      break;
    case STMT_PRINT:
      expr_resolve(s -> expr);
      break;
    case STMT_RET:
      expr_resolve(s -> expr);
      break;
    case STMT_BLOCK:
      scope_enter();
      stmt_resolve(s -> body, 1);
      scope_exit();
      break;
  }
  stmt_resolve(s -> next, which + 1);
}

void stmt_typecheck(struct stmt *s) {
  if(!s) return;
  switch(s -> kind) {
    struct type *expr;
    case STMT_DECL:
      decl_typecheck(s -> decl);
      break;
    case STMT_EXPR:
      // need to typecheck, but don't need type
      type_delete(expr_typecheck(s -> expr));
      break;
    case STMT_IF_ELSE:
      expr = expr_typecheck(s -> expr);
      if(expr -> kind != TYPE_BOOLEAN) {
        fprintf(stderr, "TYPE_ERROR: cannot use a(n) ");
        type_print(expr);
        fprintf(stderr, " as the if statement expression (currently ");
        expr_print(s -> expr);
        fprintf(stderr, ") requires a boolean\n");
        type_error_count++;
      }
      stmt_typecheck(s -> body);
      stmt_typecheck(s -> else_body);
      type_delete(expr);
      break;
    case STMT_FOR:
      type_delete(expr_typecheck(s -> init_expr));
      expr = expr_typecheck(s -> expr);
      // need to check that the middle
      // expression is actually there
      if(expr -> kind != TYPE_BOOLEAN && !expr) {
        fprintf(stderr, "TYPE_ERROR: cannot use a ");
        type_print(expr);
        fprintf(stderr, " as the middle expression requires a boolean (or an empty expression)\n");
        type_error_count++;
      }
      expr_typecheck(s -> next_expr);
      stmt_typecheck(s -> body);
      type_delete(expr);
      break;
    case STMT_WHILE:
      break;
    case STMT_PRINT:
      type_delete(expr_typecheck(s -> expr));
      break;
    case STMT_RET:
      type_delete(expr_typecheck(s -> expr));
      break;
    case STMT_BLOCK:
      stmt_typecheck(s -> body);
      break;
  }
  stmt_typecheck(s -> next);
}

