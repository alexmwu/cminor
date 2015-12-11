#include "stmt.h"
#include "library.h"
#include "../codegen/register.h"
#include "../codegen/assembly.h"
#include <stdlib.h>

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

void stmt_fprint_body(FILE *f, struct stmt *s, int indent) {
  if(s -> kind == STMT_BLOCK) {
    /*pretty print a space if the next stmt is a block*/
    fprintf(f, " ");
    stmt_fprint(f, s, indent);
  }
  else {
    /*otherwise, new line*/
    fprintf(f, "\n");
    stmt_fprint(f, s, indent + 1);
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

void stmt_fprint(FILE *f, struct stmt *s, int indent) {
  if(!s) return;
  if(s -> kind != STMT_BLOCK)
    fprint_indent(f, indent);

  switch(s -> kind) {
    case STMT_DECL:
      decl_fprint(f, s -> decl, indent - 1);
      /*don't print new line because decl always prints new lines*/
      break;
    case STMT_EXPR:
      expr_fprint(f, s -> expr);
      fprintf(f, ";\n");
      break;
    case STMT_IF_ELSE:
      fprintf(f, "if(");
      expr_fprint(f, s -> expr);
      fprintf(f, ")");
      stmt_fprint_body(f, s -> body, indent);
      if(s -> else_body) {
        fprint_indent(f, indent);
        fprintf(f, "else");
        stmt_fprint_body(f, s -> else_body, indent);
      }
      break;
    case STMT_FOR:
      fprintf(f, "for(");
      expr_fprint(f, s -> init_expr);
      fprintf(f, "; ");
      expr_fprint(f, s -> expr);
      fprintf(f, "; ");
      expr_fprint(f, s -> next_expr);
      fprintf(f, ")");
      stmt_fprint_body(f, s -> body, indent);
      break;
    case STMT_WHILE:
      /*
       *while loops are not in the CMinor spec so this should not get called.
       *if it does, it breaks immediately
       */
      break;
    case STMT_PRINT:
      fprintf(f, "print");
      if(s -> expr) {
        fprintf(f, " ");
        expr_fprint(f, s -> expr);
      }
      fprintf(f, ";\n");
      break;
    case STMT_RET:
      fprintf(f, "return");
      if(s -> expr) {
        fprintf(f, " ");
        expr_fprint(f, s -> expr);
      }
      fprintf(f, ";\n");
      break;
    case STMT_BLOCK:
      fprintf(f, "{\n");
      stmt_fprint(f, s -> body, indent + 1);
      fprint_indent(f, indent);
      fprintf(f, "}\n");
      break;
  }
  stmt_fprint(f, s -> next, indent);
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

void stmt_resolve(struct stmt *s, int *which) {
  if(!s) return;
  switch(s -> kind) {
    case STMT_DECL:
      decl_resolve(s -> decl, SYMBOL_LOCAL, which);
      (*which)++;
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
      // a new block does not equal a new stack frame
      // (so should not have new which value of 1)
      stmt_resolve(s -> body, which);
      scope_exit();
      break;
  }
  stmt_resolve(s -> next, which);
}

void stmt_typecheck(struct stmt *s, struct type *ret, int *returned) {
  if(!s) return;
  switch(s -> kind) {
    struct type *expr;
    struct expr *curr;
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
        type_fprint(stderr, expr);
        fprintf(stderr, " as the if statement expression (currently ");
        expr_fprint(stderr, s -> expr);
        fprintf(stderr, ") requires a boolean\n");
        type_error_count++;
      }
      stmt_typecheck(s -> body, ret, returned);
      stmt_typecheck(s -> else_body, ret, returned);
      type_delete(expr);
      break;
    case STMT_FOR:
      type_delete(expr_typecheck(s -> init_expr));
      expr = expr_typecheck(s -> expr);
      // need to check that the middle
      // expression is actually there
      if(expr -> kind != TYPE_BOOLEAN && !expr) {
        fprintf(stderr, "TYPE_ERROR: cannot use a ");
        type_fprint(stderr, expr);
        fprintf(stderr, " as the middle expression requires a boolean (or an empty expression)\n");
        type_error_count++;
      }
      expr_typecheck(s -> next_expr);
      stmt_typecheck(s -> body, ret, returned);
      type_delete(expr);
      break;
    case STMT_WHILE:
      break;
    case STMT_PRINT:
      curr = s -> expr;
      while(curr) {
        expr = expr_typecheck(curr);
        // pass type through new symbol
        if(!type_is_atomic(expr)) {
          fprintf(stderr, "TYPE_ERROR: cannot print (print ");
          expr_fprint(stderr, s -> expr);
          fprintf(stderr, ") a non-atomic value (");
          type_fprint(stderr, expr);
          fprintf(stderr, ")\n");
          type_error_count++;
        }
        switch(expr -> kind) {
          case TYPE_BOOLEAN:
            curr -> print_type = 1;
            break;
          case TYPE_CHARACTER:
            curr -> print_type = 2;
            break;
          case TYPE_INTEGER:
            curr -> print_type = 3;
            break;
          case TYPE_STRING:
            curr -> print_type = 4;
            break;
          case TYPE_ARRAY:
            curr -> print_type = 0;
            fprintf(stderr, "Bad entry into switch on type_kind in stmt_typecheck (case STMT_PRINT)\n");
            exit(1);
            break;
          case TYPE_ARRAY_DECL:
            curr -> print_type = 0;
            fprintf(stderr, "Bad entry into switch on type_kind in stmt_typecheck (case STMT_PRINT)\n");
            exit(1);
            break;
          case TYPE_FUNCTION:
            curr -> print_type = 0;
            fprintf(stderr, "Bad entry into switch on type_kind in stmt_typecheck (case STMT_PRINT)\n");
            exit(1);
            break;
          case TYPE_VOID:
            curr -> print_type = 0;
            fprintf(stderr, "Bad entry into switch on type_kind in stmt_typecheck (case STMT_PRINT)\n");
            exit(1);
            break;
        }
        type_delete(expr);
        curr = curr -> next;
      }
      break;
    case STMT_RET:
      // always set to 1
      *returned = 1;
      expr = expr_typecheck(s -> expr);
      if(!s -> expr) {
        type_delete(expr);
        expr = type_create(TYPE_VOID, 0, 0, 0);
      }
      if(!type_compare(expr, ret)) {
        fprintf(stderr, "TYPE_ERROR: the return statement (return ");
        expr_fprint(stderr, s -> expr);
        fprintf(stderr, ") does not match the function return type (");
        type_fprint(stderr, ret);
        fprintf(stderr, ")\n");
        type_error_count++;
      }
      type_delete(expr);
      break;
    case STMT_BLOCK:
      stmt_typecheck(s -> body, ret, returned);
      break;
  }
  stmt_typecheck(s -> next, ret, returned);
}

void stmt_call_print(struct expr *curr, FILE *f) {
  switch(curr -> print_type) {
    case 0:
      fprintf(stderr, "Expr ");
      expr_fprint(stderr, curr);
      fprintf(stderr, " has bad print_type (%d)\n", curr -> print_type);
      exit(1);
      break;
    case 1:
      expr_func_codegen(curr, "print_boolean", f);
      register_free(curr -> reg);
      break;
    case 2:
      expr_func_codegen(curr, "print_character", f);
      register_free(curr -> reg);
      break;
    case 3:
      expr_func_codegen(curr, "print_integer", f);
      register_free(curr -> reg);
      break;
    case 4:
      expr_func_codegen(curr, "print_string", f);
      register_free(curr -> reg);
      break;
    default:
      fprintf(stderr, "Expr ");
      expr_fprint(stderr, curr);
      fprintf(stderr, " has bad print_type (%d)\n", curr -> print_type);
      exit(1);
      break;
  }
}

void stmt_codegen(struct stmt *s, FILE *f) {
  if(!s) return;
  switch(s -> kind) {
    case STMT_DECL:
      decl_codegen(s -> decl, f, SYMBOL_LOCAL);
      break;
    case STMT_EXPR:
      expr_codegen(s -> expr, f);
      register_free(s -> expr -> reg);
      break;
    case STMT_IF_ELSE: {
      int done = assembly_jump_label++;
      expr_codegen(s -> expr, f);
      fprintf(f, "\tCMP $0, %s\n", register_name(s -> expr -> reg));
      register_free(s -> expr -> reg);
      if(s -> else_body) {
        int el = assembly_jump_label++;
        // jump to else
        fprintf(f, "\tJE L%d\n", el);
        stmt_codegen(s -> body, f);
        assembly_comment(f, "\t# jump to done label (don't evaluate else label)\n");
        fprintf(f, "JMP L%d\n", done);
        assembly_comment(f, "\t# else label\n");
        fprintf(f, "L%d:\n", el);
        stmt_codegen(s -> else_body, f);
        assembly_comment(f, "\t# done label\n");
        fprintf(f, "L%d:\n", done);
      }
      else {
        // jump to done
        fprintf(f, "\tJE L%d\n", done);
        stmt_codegen(s -> body, f);
        assembly_comment(f, "\t# done label\n");
        fprintf(f, "L%d:\n", done);
      }
      break;
    }
    case STMT_FOR: {
      int loop = assembly_jump_label++;
      int done = assembly_jump_label++;
      expr_codegen(s -> init_expr, f);
      register_free(s -> init_expr -> reg);
      // loop label
      assembly_comment(f, "\t# loop label\n");
      fprintf(f, "L%d:\n", loop);
      expr_codegen(s -> expr, f);
      // don't need the return value
      register_free(s -> expr -> reg);
      fprintf(f, "\tCMP $0, %s\n", register_name(s -> expr -> reg));
      assembly_comment(f, "\t# jump to done label (break condition)\n");
      fprintf(f, "\tJE L%d\n", done);
      stmt_codegen(s -> body, f);
      expr_codegen(s -> next_expr, f);
      register_free(s -> next_expr -> reg);
      assembly_comment(f, "\t# unconditional jump to next loop\n");
      fprintf(f, "JMP L%d\n", loop);
      assembly_comment(f, "\t# done label\n");
      fprintf(f, "L%d:\n", done);

      register_free(s -> expr -> reg);
      break;
    }
    case STMT_WHILE:
      fprintf(stderr, "CODEGEN_ERROR: cminor does not support while loops\n");
      exit(1);
      break;
    case STMT_PRINT: {
      struct expr *curr = s -> expr;
      while(curr) {
        expr_codegen(curr, f);

        int count = 0;

        if(ASSEMBLY_COMMENT_FLAG) {
          fprintf(f, "\t# move arg %d (in %s) into %s\n", count, register_name(curr -> reg), register_arg_names[count]);
        }

        fprintf(f, "\tMOVQ %s, %s\n", register_name(curr -> reg), register_arg_names[count++]);

        register_free(curr -> reg);
        // calls func_codegen which save ret val in reg
        // so need to free reg beforehand
        stmt_call_print(curr, f);

        curr = curr -> next;
      }
      break;
    }
    case STMT_RET:
      expr_codegen(s -> expr, f);
      if(ASSEMBLY_COMMENT_FLAG) {
        fprintf(f, "\n\t# return statement (return ");
        expr_fprint(f, s -> expr);
        fprintf(f, ")\n");
      }
      fprintf(f, "\tMOVQ %s, %%rax\n", register_name(s -> expr -> reg));
      register_free(s -> expr -> reg);

      // postamble
      assembly_comment(f, "\t### function postamble\n");
      assembly_comment(f, "\t# restore callee-saved registers\n");
      fprintf(f, "\tPOPQ %%r15\n");
      fprintf(f, "\tPOPQ %%r14\n");
      fprintf(f, "\tPOPQ %%r13\n");
      fprintf(f, "\tPOPQ %%r12\n");
      fprintf(f, "\tPOPQ %%rbx\n");

      assembly_comment(f, "\t# reset stack to base pointer\n");
      fprintf(f, "\tMOVQ %%rbp, %%rsp\n");
      assembly_comment(f, "\t# restore the old base pointer\n");
      fprintf(f, "\tPOPQ %%rbp\n");
      fprintf(f, "\tRET\n");
      break;
    case STMT_BLOCK:
      stmt_codegen(s -> body, f);
      break;
  }
  stmt_codegen(s -> next, f);
}

