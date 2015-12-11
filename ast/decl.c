#include "decl.h"
#include "../codegen/register.h"
#include "../codegen/assembly.h"
#include <stdlib.h>

struct decl *decl_create(struct expr *name, struct type *t, struct expr *v, struct stmt *c, struct decl *next) {
  struct decl *d = malloc(sizeof *d);
  d -> name = name;
  d -> type = t;
  d -> value = v;
  d -> code = c;
  d -> next = next;
  d -> symbol = 0;
  d -> num_params = 0;
  d -> num_locals = 0;
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

void decl_fprint(FILE *f, struct decl *d, int indent) {
  if(!d) return;
  fprint_indent(f, indent);
  expr_fprint(f, d -> name);
  fprintf(f, ": ");
  type_fprint(f, d -> type);
  if(d -> value) {
    fprintf(f, " = ");
    // expr_list rather than just one expr
    if(d -> value -> next) {
      fprintf(f, "{");
      expr_fprint(f, d -> value);
      fprintf(f, "}");
    }
    else {
      expr_fprint(f, d -> value);
    }
    fprintf(f, ";\n");
  }
  else if(d -> code) {
    fprintf(f, " = ");
    fprintf(f, "{\n");
    stmt_fprint(f, d -> code, indent + 1);
    fprintf(f, "}\n");
  }
  else {
    fprintf(f, ";\n");
  }
  decl_fprint(f, d -> next, indent);
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

void decl_resolve(struct decl *d, symbol_t kind, int *which) {
  if(!d) return;
  struct symbol *old = scope_curr_lookup(d -> name -> name);

  // ensure that function decls are global
  if(d -> type -> kind == TYPE_FUNCTION && kind != SYMBOL_GLOBAL) {
    fprintf(stderr, "TYPE_ERROR: functions must be declared in global scope (");
    expr_fprint(stderr, d -> name);
    fprintf(stderr, " is %s)\n", symbol_kind_string(kind));
    resolve_error_count++;
  }
  else if(old && old -> type -> kind != TYPE_FUNCTION) {
    fprintf(stderr, "%s is a redeclaration. Already declared as %s variable\n", d -> name -> name, symbol_kind_string(old -> kind));
    resolve_error_count++;
  }
  // implicit that symbol type kind if function
  else if(old) {
    // function already been declared
    if(old -> which == 1) {
        if(d -> code) {
          // a little bit of typechecking in
          // name resolution to ensure that
          // definition matches declaration
          if(!type_compare(d -> type, old -> type)) {
            fprintf(stderr, "TYPE_ERROR: prototype in definition of function ");
            expr_fprint(stderr, d -> name);
            fprintf(stderr, " (");
            type_fprint(stderr, d -> type);
            fprintf(stderr, ") does not match prototype in original declaration (");
            type_fprint(stderr, old -> type);
            fprintf(stderr, ")\n");
            // both resolution and type error
            // but pick one (pick the one that
            // fails earlier)
            resolve_error_count++;
          }
          old -> which = 2;
        }
        // redeclaration of function
        else {
          fprintf(stderr, "%s is a redeclaration. Already declared as %s variable\n", d -> name -> name, symbol_kind_string(old -> kind));
          resolve_error_count++;
        }
    }
    // function already declared/defined
    else if(old -> which == 2) {
      fprintf(stderr, "%s is a redeclaration/redefinition. Already declared as %s variable\n", d -> name -> name, symbol_kind_string(old -> kind));
      resolve_error_count++;
    }
  }
  else {
    struct symbol *sym = symbol_create(kind, d -> type, d -> name);
    sym -> orig_decl = d;
    if(d -> type -> kind == TYPE_FUNCTION) {
      // 2 represents that a function has
      // been defined and declared
      if(d -> code) {
        sym -> which = 2;
      }
      // 1 means that a function has only
      // been defined
      else {
        sym -> which = 1;
      }
    }
    else {
      sym -> which = *which;
    }
    d -> symbol = sym;
    scope_bind(d -> name -> name, sym);
  }
  expr_resolve(d -> value);
  // want to typecheck all code, even new ones
  if(d -> code) {
    scope_enter();
    d -> num_params = param_list_resolve(d -> type -> params, 1);
    int new_scope = 1;
    stmt_resolve(d -> code, &new_scope);
    d -> num_locals = new_scope - 1;
    scope_exit();
  }
  int new_decl = 1;
  decl_resolve(d -> next, kind, &new_decl);
}

void decl_typecheck(struct decl *d) {
  if(!d) return;

  // need to place this first
  if(d -> type -> kind == TYPE_ARRAY_DECL || d -> type -> kind == TYPE_ARRAY) {
    if(d -> value && d -> value -> kind != EXPR_ARR_INITLIST) {
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
        type_fprint(stderr, curr);
        fprintf(stderr, ")\n");
        type_error_count++;
      }
      else if(!expr_is_num_constant(d -> type -> expr)) {
        fprintf(stderr, "TYPE_ERROR: arrays (");
        expr_fprint(stderr, d -> name);
        fprintf(stderr, ") need to have constant integer type declarations (has type of ");
        type_fprint(stderr, d -> type);
        fprintf(stderr, ")\n");
        type_error_count++;
      }
      else {
        int count = d -> type -> expr -> literal_value;
        // stop processing if count <= 0
        if(count <= 0) {
          fprintf(stderr, "TYPE_ERROR: declared size of array ");
          expr_fprint(stderr, d -> name);
          fprintf(stderr, " is not a positive number (%d)\n", count);
          type_error_count++;
        }
        // calling d -> value -> next_list
        // because top level is useless
        // also need to check if d -> value
        // exists (otherwise segfault)
        else if(d -> value)
          expr_arr_init_typecheck(d -> name, d -> value -> next_list, d -> type, curr, count);
      }
    }
  }

  struct type *value;
  // bit hacky, but want to check expr, not decl
  // (though they should be tied, they may not be)
  if(d -> value && d -> value -> kind == EXPR_ARR_INITLIST) {
    value = 0;
  }
  // value variable used for rest of typecheck
  else
    value = expr_typecheck(d -> value);

  // check that expr exists (and it is not a
  // special case of initializer list)
  if(value && d -> value -> kind != EXPR_ARR_INITLIST && !type_compare(d -> type, value)) {
    fprintf(stderr, "TYPE_ERROR: type declaration value (");
    type_fprint(stderr, value);
    fprintf(stderr, ") for %s does not match declared value (", d -> name -> name);
    type_fprint(stderr, d -> type);
    fprintf(stderr, ")\n");
    type_error_count++;
  }
  // check that global variables have constant type declarations
  if(value && d -> value -> kind != EXPR_ARR_INITLIST && d -> symbol -> kind == SYMBOL_GLOBAL && !expr_is_constant(d -> value)) {
    fprintf(stderr, "TYPE_ERROR: global variables (");
    expr_fprint(stderr, d -> name);
    fprintf(stderr, ") need to have constant type declarations (has type of ");
    type_fprint(stderr, value);
    fprintf(stderr, ")\n");
    type_error_count++;
  }

  type_delete(value);
  int returnedValue = 0;
  int *returned = &returnedValue;
  if(d -> type -> kind == TYPE_FUNCTION) {
    stmt_typecheck(d -> code, d -> type -> subtype, returned);
    // if the function did not return anything
    // and it does not return void and it has
    // a declaration (rather than definition)
    if(d -> code && !(*returned) && d -> type -> subtype -> kind != TYPE_VOID) {
      fprintf(stderr, "TYPE_ERROR: no return in a function (");
      expr_fprint(stderr, d -> name);
      fprintf(stderr, ") of return type ");
      type_fprint(stderr, d -> type -> subtype);
      fprintf(stderr, "\n");
      type_error_count++;
    }
  }
  else {
    stmt_typecheck(d -> code, 0, returned);
  }
  decl_typecheck(d -> next);
}

void decl_codegen(struct decl *d, FILE *f, symbol_t kind) {
  if(!d) return;
  if(d -> type -> kind == TYPE_ARRAY_DECL) {
    fprintf(stderr, "CODEGEN_ERROR: cminor does not have an array implementation\n");
    exit(1);
  }

  // implied global decl or it would have failed in typecheck
  if(d -> type -> kind == TYPE_FUNCTION) {
#ifdef __linux__
    fprintf(f, ".globl %s\n", d -> name -> name);
    fprintf(f, "%s:\n", d -> name -> name);
#elif __APPLE__
    fprintf(f, ".globl _%s\n", d -> name -> name);
    fprintf(f, "_%s:\n", d -> name -> name);
#else
    fprintf(f, ".globl %s\n", d -> name -> name);
    fprintf(f, "%s:\n", d -> name -> name);
#endif
    // preamble of function (setting up stack)
    assembly_comment(f, "\t### function preamble\n");
    assembly_comment(f, "\t# save the base pointer\n");
    fprintf(f, "\tPUSHQ %%rbp\n");
    assembly_comment(f, "\t# set new base pointer to rsp\n");
    fprintf(f, "\tMOVQ %%rsp, %%rbp\n");
    assembly_arg_stack_alloc(f, d -> num_params);
    if(ASSEMBLY_COMMENT_FLAG) {
      fprintf(f, "\n\t # allocate %d local variables\n", d -> num_locals);
    }
    // allocate a byte per local (8)
#ifdef __linux__
    fprintf(f, "\tSUBQ $%d, %%rsp\n", d -> num_locals * 8);
#elif __APPLE__
    // num params, locals, and callee-saved regs
    int num_stack_vars = d -> num_params + d -> num_locals + REGISTER_NUM_CALLEE_SAVED;
    // add an unused local if the num is odd
    // (keep stack aligned on 16 byte boundary)
    int num_locals = (num_stack_vars % 2 == 0) ? d -> num_locals : (d -> num_locals + 1);
    fprintf(f, "\tSUBQ $%d, %%rsp\n", num_locals * 8);
#else
    fprintf(f, "\tSUBQ $%d, %%rsp\n", d -> num_locals * 8);
#endif
    assembly_comment(f, "\n\t# save callee-saved registers\n");
    fprintf(f, "\tPUSHQ %%rbx\n");
    fprintf(f, "\tPUSHQ %%r12\n");
    fprintf(f, "\tPUSHQ %%r13\n");
    fprintf(f, "\tPUSHQ %%r14\n");
    fprintf(f, "\tPUSHQ %%r15\n");
    assembly_comment(f, "\n\t### function body\n\n");
    stmt_codegen(d -> code, f);
  }
  else if(kind == SYMBOL_GLOBAL) {
    /*fprintf(f, ".data\n");*/
#ifdef __linux__
    fprintf(f, ".globl %s\n", d -> name -> name);
#elif __APPLE__
    fprintf(f, ".globl _%s\n", d -> name -> name);
#else
    fprintf(f, ".globl %s\n", d -> name -> name);
#endif
    fprintf(f, ".data\n");
#ifdef __linux__
    fprintf(f, "%s:\n", d -> name -> name);
#elif __APPLE__
    fprintf(f, "_%s:\n", d -> name -> name);
#else
    fprintf(f, "%s:\n", d -> name -> name);
#endif
    if(d -> value) {
      // should have already been typechecked
      if(d -> value -> kind == EXPR_TRUE) {
        fprintf(f, "\t.quad 1\n");
      }
      else if(d -> value -> kind == EXPR_FALSE) {
        fprintf(f, "\t.quad 0\n");
      }
      else if(d -> value -> kind == EXPR_INTLIT) {
        fprintf(f, "\t.quad %d\n", d -> value -> literal_value);
      }
      else if(d -> value -> kind == EXPR_CHARLIT) {
        fprintf(f, "\t.quad %d\n", d -> value -> char_literal);
      }
      else if(d -> value -> kind == EXPR_STRLIT) {
        char *val = assembly_string_out((char *) d -> value -> string_literal);
        fprintf(f, "\t.string \"%s\"\n", val);
        free(val);
      }
      /*expr_codegen(d -> value, f);*/
    }
    else {
      if(d -> type -> kind == TYPE_BOOLEAN) {
        fprintf(f, "\t.quad 0\n");
      }
      else if(d -> type -> kind == TYPE_INTEGER) {
        fprintf(f, "\t.quad 0\n");
      }
      else if(d -> type -> kind == TYPE_CHARACTER) {
        fprintf(f, "\t.quad ' '\n");
      }
      else if(d -> type -> kind == TYPE_STRING) {
        fprintf(f, "\t.string \"\"\n");
      }
    }
    fprintf(f, ".text\n");
  }
  // else it is another declaration of type local
  else {
    if(d -> value) {
      char *val;
      if(d -> value -> kind == EXPR_STRLIT) {
        fprintf(f, ".data\n");
        d -> value -> str_num = expr_num_str++;
        fprintf(f, "STR%d:\n", d -> value -> str_num);
        val = assembly_string_out((char *) d -> value -> string_literal);
        fprintf(f, "\t.string \"%s\"\n", val);
        free(val);
        fprintf(f, ".text\n");
      }
      else {
        char *name = symbol_code(d -> symbol);
        expr_codegen(d -> value, f);
        fprintf(f, "\tMOVQ %s, %s\n", register_name(d -> value -> reg), name);
        free(name);
      }
    }
  }
  decl_codegen(d -> next, f, kind);
}

