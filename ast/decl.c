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

void decl_resolve(struct decl *d, symbol_t kind, int which) {
  if(!d) return;
  struct symbol *sym = symbol_create(kind, d -> type, d -> name);
  sym -> which = which;
  d -> symbol = sym;
  struct symbol *old = scope_lookup(d -> name -> name);
  if(old) {
    fprintf(stderr, "%s is a redeclaration. Already declared as %s variable\n", d -> name -> name, symbol_kind_print(old -> kind));
    resolve_error_count++;
  }
  else {
    scope_bind(d -> name -> name, sym);
  }
  expr_resolve(d -> value);
  if(d -> code) {
    scope_enter();
    param_list_resolve(d -> type -> params, 1);
    stmt_resolve(d -> code, 1);
    scope_exit();
  }
  decl_resolve(d -> next, kind, which);
}

void decl_typecheck(struct decl *d) {
  if(!d) return;
  struct type *value = expr_typecheck(d -> value);
  // check that expr exists (and it is not a
  // special case of initializer list)
  if(value && d -> value -> kind != EXPR_ARR_INITLIST && !type_compare(d -> type, value)) {
    fprintf(stderr, "TYPE_ERROR: type declaration value (");
    type_print(value);
    fprintf(stderr, ") for %s does not match declared value (", d -> name -> name);
    type_print(d -> type);
    fprintf(stderr, ")\n");
    type_error_count++;
  }
  // check that global variables have constant type declarations
  if(value && d -> value -> kind != EXPR_ARR_INITLIST && d -> symbol -> kind == SYMBOL_GLOBAL && !expr_is_constant(d -> value)) {
    fprintf(stderr, "TYPE_ERROR: global variables (");
    expr_print(d -> name);
    fprintf(stderr, ") need to have constant type declarations (has type of ");
    type_print(value);
    fprintf(stderr, ")\n");
    type_error_count++;
  }

  if(d -> type -> kind == TYPE_ARRAY_DECL) {
    if(d -> value -> kind != EXPR_ARR_INITLIST) {
      fprintf(stderr, "Bad initialization: array declaration expression for (");
      expr_print(d -> name);
      fprintf(stderr, "does not have kind EXPR_ARR_INITLIST\n");
      // program (not user) error
      exit(1);
    }
    else {
      struct type *curr = d -> type;
      while(curr -> subtype) {
        curr = curr -> subtype;
      }
      if(!type_is_atomic(curr)) {
        fprintf(stderr, "TYPE_ERROR: the base type (final nested type) of the array is not an atomic type (has type ");
        type_print(curr);
        fprintf(stderr, ")\n");
        type_error_count++;
      }
      else {
        int count = d -> type -> expr -> literal_value;
        if(count <= 0) {
          fprintf(stderr, "TYPE_ERROR: declared size of array ");
          expr_print(d -> name);
          fprintf(stderr, " is not a positive number (%d)\n", count);
          type_error_count++;
        }
        else
          expr_arr_init_typecheck(d -> name, d -> value, curr, count);
      }
    }
  }
/*
 *  // check initializer lists and expression
 *  if(d -> type -> kind == TYPE_ARRAY_DECL) {
 *    struct expr *v = d -> type -> expr;
 *    if(v -> kind != EXPR_INTLIT) {
 *      // not constant; print error
 *      fprintf(stderr, "TYPE_ERROR: must use constant integers when declaring arrays (");
 *      expr_print(v);
 *      fprintf(stderr, ")\n");
 *      type_error_count++;
 *    }
 *    struct type *t = d -> type;
 *
 *    decl_array_initializer_typecheck(d -> type, d -> value);
 *  }
 */
  type_delete(value);
  if(d -> type -> kind == TYPE_FUNCTION) {
    stmt_typecheck(d -> code, d -> type -> subtype);
  }
  else {
    stmt_typecheck(d -> code, 0);
  }
  decl_typecheck(d -> next);
}

