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
  e -> arr_next = 0;
  e -> name = 0;
  e -> string_literal = 0;
  e -> symbol = 0;
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
  e -> literal_value = c;
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
  struct expr *curr_arr;
  switch(e -> kind) {
    case EXPR_PLUS:
      expr_print(e -> left);
      printf("+");
      expr_print(e -> right);
      break;
    case EXPR_MIN:
      if(!e -> left) {
        printf("-");
        expr_print(e -> right);
        break;
      }
      expr_print(e -> left);
      printf("-");
      expr_print(e -> right);
      break;
    case EXPR_MUL:
      expr_print(e -> left);
      printf("*");
      expr_print(e -> right);
      break;
    case EXPR_DIV:
      expr_print(e -> left);
      printf("/");
      expr_print(e -> right);
      break;
    case EXPR_MOD:
      expr_print(e -> left);
      printf("%%");
      expr_print(e -> right);
      break;
    case EXPR_PLUSPLUS:
      if(e -> left && e -> right) {
        fprintf(stderr, "Pre/Post error: 2 exprs (should just be one)\n");
        exit(1);
      }
      else if(e -> left) {
        expr_print(e -> left);
        printf("++");
      }
      else if(e -> right) {
        printf("++");
        expr_print(e -> right);
      }
      else {
        fprintf(stderr, "Pre/Post error: 0 exprs (should have one)\n");
        exit(1);
      }
      break;
    case EXPR_MINMIN:
      if(e -> left && e -> right) {
        fprintf(stderr, "Pre/Post error: 2 exprs (should just be one)\n");
        exit(1);
      }
      else if(e -> left) {
        expr_print(e -> left);
        printf("--");
      }
      else if(e -> right) {
        printf("--");
        expr_print(e -> right);
      }
      else {
        fprintf(stderr, "Pre/Post error: 0 exprs (should have one)\n");
        exit(1);
      }
      break;
    case EXPR_EXP:
      expr_print(e -> left);
      printf("^");
      expr_print(e -> right);
      break;
    case EXPR_LT:
      expr_print(e -> left);
      printf("<");
      expr_print(e -> right);
      break;
    case EXPR_LE:
      expr_print(e -> left);
      printf("<=");
      expr_print(e -> right);
      break;
    case EXPR_GT:
      expr_print(e -> left);
      printf(">");
      expr_print(e -> right);
      break;
    case EXPR_GE:
      expr_print(e -> left);
      printf(">=");
      expr_print(e -> right);
      break;
    case EXPR_EQEQ:
      expr_print(e -> left);
      printf("==");
      expr_print(e -> right);
      break;
    case EXPR_NE:
      expr_print(e -> left);
      printf("!=");
      expr_print(e -> right);
      break;
    case EXPR_AND:
      expr_print(e -> left);
      printf("&&");
      expr_print(e -> right);
      break;
    case EXPR_OR:
      expr_print(e -> left);
      printf("||");
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
      curr_arr = e -> arr_next;
      while(curr_arr) {
        printf("[");
        expr_print(curr_arr);
        printf("]");
        curr_arr = curr_arr -> arr_next;
      }
      printf(" = ");
      expr_print(e -> right);
      break;
    case EXPR_EQ:
      expr_print(e -> left);
      printf(" = ");
      expr_print(e -> right);
      break;
    case EXPR_ARR:
      expr_print(e -> left);
      curr_arr = e -> arr_next;
      while(curr_arr) {
        printf("[");
        expr_print(curr_arr);
        printf("]");
        curr_arr = curr_arr -> arr_next;
      }
      break;
    case EXPR_GROUP:
      printf("(");
      /*
       *Grouping is defined as one expression around paren
       *(so just print e->right which should be just one expr)
       */
      expr_print(e -> right);
      printf(")");
      break;
    case EXPR_FUNC:
      expr_print(e -> left);
      printf("(");
      expr_print(e -> right);
      printf(")");
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
  /*print expr_lists*/
  if(e -> next) {
    printf(", ");
    expr_print(e -> next);
  }
}

void expr_free(struct expr *e) {
  if(!e) return;
  expr_free(e -> left);
  expr_free(e -> right);
  expr_free(e -> next);
  expr_free(e -> arr_next);
  free((char *) e -> name);
  // should not free as decl "owns" the object
  /*symbol_free(e -> symbol);*/
  free((char *) e -> string_literal);
  free(e);
}

void expr_resolve(struct expr *e) {
  if(!e) return;
  expr_resolve(e -> left);
  expr_resolve(e -> right);
  expr_resolve(e -> next);
  if(e -> kind == EXPR_IDENT) {
    struct symbol *s = scope_lookup(e -> name);
    if(s) {
      e -> symbol = s;
      if(s -> kind == SYMBOL_GLOBAL)
        printf("%s resolves to global %s\n", e -> name, s -> name -> name);
      else {
        printf("%s resolves to %s %d\n", e -> name, symbol_kind_print(s -> kind), s -> which);
      }
    }
    else {
      fprintf(stderr, "RESOLVE_ERROR: %s is not defined\n", e -> name);
      resolve_error_count++;
    }
  }
}

void expr_typecheck_err_print(FILE *f, struct expr *e) {
  fprintf(f, " (");
  expr_print(e);
  fprintf(f, ")\n");
}

struct type *expr_arith_typecheck(struct expr *e, int which) {
  char *arith_type = 0;
  switch(which) {
    case 0:
      arith_type = "add";
      break;
    case 1:
      arith_type = "subtract";
      break;
    case 2:
      arith_type = "multiply";
      break;
    case 3:
      arith_type = "divide";
      break;
    case 4:
      arith_type = "modulo";
      break;
    case 5:
      arith_type = "raise";
  }
  struct type *left, *right;
  left = expr_typecheck(e -> left);
  right = expr_typecheck(e -> right);
  if(left -> kind != TYPE_INTEGER || right -> kind != TYPE_INTEGER) {
    fprintf(stderr, "TYPE_ERROR: cannot %s ", arith_type);
    type_print(left);
    fprintf(stderr, " with ");
    type_print(right);
    expr_typecheck_err_print(stderr, e);
    type_error_count++;
  }
  type_delete(left);
  type_delete(right);
  return type_create(TYPE_INTEGER, 0, 0, 0);
}

struct type *expr_comp_typecheck(struct expr *e, int which) {
  char *comp_type = 0;
  switch(which) {
    case 0:
      comp_type = "less than";
      break;
    case 1:
      comp_type = "less than or equal";
      break;
    case 2:
      comp_type = "greater than";
      break;
    case 3:
      comp_type = "greater than or equal";
      break;
  }
  struct type *left, *right;
  left = expr_typecheck(e -> left);
  right = expr_typecheck(e -> right);
  if(left -> kind != TYPE_INTEGER || right -> kind != TYPE_INTEGER) {
    fprintf(stderr, "TYPE_ERROR: cannot apply %s on ", comp_type);
    type_print(left);
    fprintf(stderr, " and ");
    type_print(right);
    expr_typecheck_err_print(stderr, e);
    type_error_count++;
  }
  type_delete(left);
  type_delete(right);
  return type_create(TYPE_BOOLEAN, 0, 0, 0);
}

struct type *expr_bool_typecheck(struct expr *e, int which) {
  char *boolop_type = 0;
  switch(which) {
    case 0:
      boolop_type  = "and";
      break;
    case 1:
      boolop_type  = "or";
      break;
    case 2:
      boolop_type  = "not";
      break;
  }
  struct type *left, *right;
  right = expr_typecheck(e -> right);
  if(which == 2) {
    if(right -> kind != TYPE_BOOLEAN) {
      fprintf(stderr, "TYPE_ERROR: cannot apply %s on ", boolop_type);
      type_print(right);
      expr_typecheck_err_print(stderr, e);
      type_error_count++;
    }
    type_delete(right);
    return type_create(TYPE_BOOLEAN, 0, 0, 0);
  }
  left = expr_typecheck(e -> left);
  if(left -> kind != TYPE_BOOLEAN || right -> kind != TYPE_BOOLEAN) {
    fprintf(stderr, "TYPE_ERROR: cannot apply %s on ", boolop_type);
    type_print(left);
    fprintf(stderr, " and ");
    type_print(right);
    expr_typecheck_err_print(stderr, e);
    type_error_count++;
  }
  type_delete(left);
  type_delete(right);
  return type_create(TYPE_BOOLEAN, 0, 0, 0);
}

struct type *expr_eq_typecheck(struct expr *e, int which) {
  char *eq_type = 0;
  switch(which) {
    case 0:
      eq_type  = "equals equals";
      break;
    case 1:
      eq_type  = "not equal";
      break;
  }
  struct type *left, *right;
  left = expr_typecheck(e -> left);
  right = expr_typecheck(e -> right);
  if(left -> kind == TYPE_FUNCTION || right -> kind == TYPE_FUNCTION) {
    fprintf(stderr, "TYPE_ERROR: cannot use %s on a function", eq_type);
    expr_typecheck_err_print(stderr, e);
    type_error_count++;
  }
  else if(left -> kind == TYPE_ARRAY || right -> kind == TYPE_ARRAY) {
    fprintf(stderr, "TYPE_ERROR: cannot use %s on an array", eq_type);
    expr_typecheck_err_print(stderr, e);
    type_error_count++;
  }
  else if(left -> kind == TYPE_ARRAY_DECL || right -> kind == TYPE_ARRAY_DECL) {
    fprintf(stderr, "TYPE_ERROR: cannot use %s on an array", eq_type);
    expr_typecheck_err_print(stderr, e);
    type_error_count++;
  }
  else if(left -> kind != right -> kind) {
    fprintf(stderr, "TYPE_ERROR cannot apply %s on ", eq_type);
    type_print(left);
    fprintf(stderr, " and ");
    type_print(right);
    expr_typecheck_err_print(stderr, e);
    type_error_count++;
  }
  return type_create(TYPE_BOOLEAN, 0, 0, 0);
}

int expr_is_constant(struct expr *e) {
  if(e -> kind == EXPR_TRUE || e -> kind == EXPR_FALSE || e -> kind == EXPR_INTLIT || e -> kind == EXPR_CHARLIT || e -> kind == EXPR_STRLIT || e -> kind == EXPR_IDENT) {
    return 1;
  }
  return 0;
}

// checks if number of indices into array
// is correct; call with symbol -> type
// of array ident and array expression root
struct type *expr_arr_indexcheck(struct type *t, struct expr *e) {
  struct type *curr_type = t -> subtype;
  struct expr *a_next = e;
  while(a_next) {
    // means that there were no more nested
    // subtypes in the declaration
    if(!curr_type) {
      fprintf(stderr, "TYPE_ERROR: there are fewer nested arrays (");
      type_print(t);
      fprintf(stderr, ") in the declaration than the assignment (");
      expr_print(e);
      fprintf(stderr, ") gives\n");
      type_error_count++;
      return 0;
    }
    a_next = a_next -> arr_next;
    // return current type if both end at
    // same time
    if(!curr_type -> subtype && !a_next) {
      return type_copy(curr_type);
    }
    curr_type = curr_type -> subtype;
  }
  // if there is left over subtype, that is
  // ok (it will evaluate to an array type)
  return type_copy(curr_type);
}

// 0 is standard assignment, 1 is array assign
struct type *expr_assign_typecheck(struct expr *e, int which) {
  struct type *left, *right, *arr_next;
  left = expr_typecheck(e -> left);
  right = expr_typecheck(e -> right);
  if(which == 1) {
    if(left -> kind != TYPE_ARRAY || left -> kind != TYPE_ARRAY_DECL) {
      fprintf(stderr, "TYPE_ERROR: cannot use array assignment on non-array type ");
      type_print(left);
      expr_typecheck_err_print(stderr, e);
      type_error_count++;
    }
    else {
      // just return the type of the expr
      arr_next = expr_arr_indexcheck(left, e);
      if(!expr_arr_indexcheck(left, e)) {
        return type_copy(right);
      }
    }
    type_delete(left);
    type_delete(right);
    return type_copy(right);
  }
  else {
    if(left -> kind == TYPE_FUNCTION) {
      fprintf(stderr, "TYPE_ERROR: cannot use assignment operator on a function (%s)\n", e -> left -> name);
      type_error_count++;
      type_delete(left);
      return type_copy(right);
    }
    else {
      type_delete(right);
      return type_copy(left);
    }
  }
}

// need to type_delete the result
struct type *expr_typecheck(struct expr *e) {
  if(!e) return 0;
  switch(e -> kind) {
    struct type *left, *right, *next;
    // TODO: add a type_file_print function
    // it allows printing to a file
    // used for printing to stderr
    case EXPR_PLUS:
      return expr_arith_typecheck(e, 0);
    case EXPR_MIN:
      // unary minus
      if(!e -> left) {
        right = expr_typecheck(e -> right);
        if(right -> kind != TYPE_INTEGER) {
          fprintf(stderr, "TYPE_ERROR: unary minus only works on integers, not ");
          type_print(right);
          expr_typecheck_err_print(stderr, e);
          type_error_count++;
        }
        type_delete(right);
        return type_create(TYPE_INTEGER, 0, 0, 0);
      }
      return expr_arith_typecheck(e, 1);
    case EXPR_MUL:
      return expr_arith_typecheck(e, 2);
    case EXPR_DIV:
      return expr_arith_typecheck(e, 3);
    case EXPR_MOD:
      return expr_arith_typecheck(e, 4);
    case EXPR_PLUSPLUS:
      if(e -> right) {
        fprintf(stderr, "Error: cminor does not support preincrement\n");
        exit(1);
      }
      left = expr_typecheck(e -> left);
      if(left -> kind != TYPE_INTEGER) {
        fprintf(stderr, "TYPE_ERROR: cannot use postincrement on a ");
        type_print(left);
        expr_typecheck_err_print(stderr, e);
        type_error_count++;
      }
      type_delete(left);
      return type_create(TYPE_INTEGER, 0, 0, 0);
    case EXPR_MINMIN:
      if(e -> right) {
        fprintf(stderr, "Error: cminor does not support predecrement\n");
        exit(1);
      }
      left = expr_typecheck(e -> left);
      if(left -> kind != TYPE_INTEGER) {
        fprintf(stderr, "TYPE_ERROR: cannot use postdecrement on a ");
        type_print(left);
        expr_typecheck_err_print(stderr, e);
        type_error_count++;
      }
      type_delete(left);
      return type_create(TYPE_INTEGER, 0, 0, 0);
    case EXPR_EXP:
      return expr_arith_typecheck(e, 5);
    case EXPR_LT:
      return expr_comp_typecheck(e, 0);
    case EXPR_LE:
      return expr_comp_typecheck(e, 1);
    case EXPR_GT:
      return expr_comp_typecheck(e, 2);
    case EXPR_GE:
      return expr_comp_typecheck(e, 3);
    case EXPR_EQEQ:
      return expr_eq_typecheck(e, 0);
    case EXPR_NE:
      return expr_eq_typecheck(e, 1);
    case EXPR_AND:
      return expr_bool_typecheck(e, 0);
    case EXPR_OR:
      return expr_bool_typecheck(e, 1);
    case EXPR_NOT:
      return expr_bool_typecheck(e, 2);
    case EXPR_EQ:
      return expr_assign_typecheck(e, 0);
    case EXPR_ARREQ:
      return expr_assign_typecheck(e, 1);
    case EXPR_ARR:
      left = expr_arr_indexcheck(e -> symbol -> type, e);
      // just return last type in arr subtypes
      if(!left) {
        next = e -> symbol -> type;
        while(next -> subtype) {
          next = next -> subtype;
        }
        type_delete(left);
        return type_copy(next);
      }
      return left;
    case EXPR_GROUP:
      return expr_typecheck(e -> right);
    case EXPR_FUNC:
      if(e -> left -> symbol -> type -> kind != TYPE_FUNCTION) {
        fprintf(stderr, "TYPE_ERROR: identifier (%s) for function call is not a function\n", e -> left -> name);
        type_error_count++;
      }
      // check that params match args
      param_list_typecheck(e -> left -> symbol -> type -> params, e -> right, e -> left -> name);
        // return function return type
      return type_copy(e -> left -> symbol -> type -> subtype);
    case EXPR_TRUE:
      return type_create(TYPE_BOOLEAN, 0, 0, 0);
    case EXPR_FALSE:
      return type_create(TYPE_BOOLEAN, 0, 0, 0);
    case EXPR_INTLIT:
      return type_create(TYPE_INTEGER, 0, 0, 0);
    case EXPR_CHARLIT:
      return type_create(TYPE_CHARACTER, 0, 0, 0);
    case EXPR_STRLIT:
      return type_create(TYPE_STRING, 0, 0, 0);
    case EXPR_IDENT:
      return type_copy(e -> symbol -> type);
  }
}

