#include "expr.h"
#include "library.h"
#include "../codegen/register.h"
#include "../codegen/assembly.h"
#include <stdlib.h>

struct expr *expr_create(expr_t kind, struct expr *left, struct expr *right, struct expr *next) {
  struct expr *e = malloc(sizeof *e);
  e -> kind = kind;
  e -> left = left;
  e -> right = right;
  e -> next = next;
  e -> arr_next = 0;
  e -> next_list = 0;
  e -> name = 0;
  e -> string_literal = 0;
  // no string number (for codegen)
  e -> str_num = -1;
  e -> symbol = 0;
  e -> print_type = 0;
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
    case EXPR_ARR_INITLIST:
      printf("{");
      /*print nested array_initlist*/
      expr_print(e -> left);
      /*
       *print expr list if one exists (should
       *only be at end of linked list created
       *by following the left pointer)
       */
      expr_print(e -> right);
      printf("}");
      /*print array initializer lists*/
      curr_arr = e -> next_list;
      while(curr_arr) {
        printf(", ");
        expr_print(e -> next_list);
        curr_arr = curr_arr -> next_list;
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
      printf("'");
      print_character(e -> char_literal);
      printf("'");
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

void expr_fprint(FILE *f, struct expr *e) {
  if(!e) return;
  struct expr *curr_arr;
  switch(e -> kind) {
    case EXPR_PLUS:
      expr_fprint(f, e -> left);
      fprintf(f, "+");
      expr_fprint(f, e -> right);
      break;
    case EXPR_MIN:
      if(!e -> left) {
        fprintf(f, "-");
        expr_fprint(f, e -> right);
        break;
      }
      expr_fprint(f, e -> left);
      fprintf(f, "-");
      expr_fprint(f, e -> right);
      break;
    case EXPR_MUL:
      expr_fprint(f, e -> left);
      fprintf(f, "*");
      expr_fprint(f, e -> right);
      break;
    case EXPR_DIV:
      expr_fprint(f, e -> left);
      fprintf(f, "/");
      expr_fprint(f, e -> right);
      break;
    case EXPR_MOD:
      expr_fprint(f, e -> left);
      fprintf(f, "%%");
      expr_fprint(f, e -> right);
      break;
    case EXPR_PLUSPLUS:
      if(e -> left && e -> right) {
        fprintf(stderr, "Pre/Post error: 2 exprs (should just be one)\n");
        exit(1);
      }
      else if(e -> left) {
        expr_fprint(f, e -> left);
        fprintf(f, "++");
      }
      else if(e -> right) {
        fprintf(f, "++");
        expr_fprint(f, e -> right);
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
        expr_fprint(f, e -> left);
        fprintf(f, "--");
      }
      else if(e -> right) {
        fprintf(f, "--");
        expr_fprint(f, e -> right);
      }
      else {
        fprintf(stderr, "Pre/Post error: 0 exprs (should have one)\n");
        exit(1);
      }
      break;
    case EXPR_EXP:
      expr_fprint(f, e -> left);
      fprintf(f, "^");
      expr_fprint(f, e -> right);
      break;
    case EXPR_LT:
      expr_fprint(f, e -> left);
      fprintf(f, "<");
      expr_fprint(f, e -> right);
      break;
    case EXPR_LE:
      expr_fprint(f, e -> left);
      fprintf(f, "<=");
      expr_fprint(f, e -> right);
      break;
    case EXPR_GT:
      expr_fprint(f, e -> left);
      fprintf(f, ">");
      expr_fprint(f, e -> right);
      break;
    case EXPR_GE:
      expr_fprint(f, e -> left);
      fprintf(f, ">=");
      expr_fprint(f, e -> right);
      break;
    case EXPR_EQEQ:
      expr_fprint(f, e -> left);
      fprintf(f, "==");
      expr_fprint(f, e -> right);
      break;
    case EXPR_NE:
      expr_fprint(f, e -> left);
      fprintf(f, "!=");
      expr_fprint(f, e -> right);
      break;
    case EXPR_AND:
      expr_fprint(f, e -> left);
      fprintf(f, "&&");
      expr_fprint(f, e -> right);
      break;
    case EXPR_OR:
      expr_fprint(f, e -> left);
      fprintf(f, "||");
      expr_fprint(f, e -> right);
      break;
    case EXPR_NOT:
      fprintf(f, "!");
      if(!e -> right) {
        fprintf(stderr, "NOT Error: no right expression\n");
        exit(1);
      }
      expr_fprint(f, e -> right);
      break;
    case EXPR_ARREQ:
      /*
       *EXPR_ARREQ has left of ident, right of expr (for index),
       *and next of expr (what it is set to).
       */
      expr_fprint(f, e -> left);
      curr_arr = e -> arr_next;
      while(curr_arr) {
        fprintf(f, "[");
        expr_fprint(f, curr_arr);
        fprintf(f, "]");
        curr_arr = curr_arr -> arr_next;
      }
      fprintf(f, " = ");
      expr_fprint(f, e -> right);
      break;
    case EXPR_EQ:
      expr_fprint(f, e -> left);
      fprintf(f, " = ");
      expr_fprint(f, e -> right);
      break;
    case EXPR_ARR:
      expr_fprint(f, e -> left);
      curr_arr = e -> arr_next;
      while(curr_arr) {
        fprintf(f, "[");
        expr_fprint(f, curr_arr);
        fprintf(f, "]");
        curr_arr = curr_arr -> arr_next;
      }
      break;
    case EXPR_ARR_INITLIST:
      fprintf(f, "{");
      /*print nested array_initlist*/
      expr_fprint(f, e -> left);
      /*
       *print expr list if one exists (should
       *only be at end of linked list created
       *by following the left pointer)
       */
      expr_fprint(f, e -> right);
      fprintf(f, "}");
      /*print array initializer lists*/
      curr_arr = e -> next_list;
      while(curr_arr) {
        fprintf(f, ", ");
        expr_fprint(f, e -> next_list);
        curr_arr = curr_arr -> next_list;
      }
      break;
    case EXPR_GROUP:
      fprintf(f, "(");
      /*
       *Grouping is defined as one expression around paren
       *(so just print e->right which should be just one expr)
       */
      expr_fprint(f, e -> right);
      fprintf(f, ")");
      break;
    case EXPR_FUNC:
      expr_fprint(f, e -> left);
      fprintf(f, "(");
      expr_fprint(f, e -> right);
      fprintf(f, ")");
      break;
    case EXPR_TRUE:
      fprint_boolean(f, 1);
      break;
    case EXPR_FALSE:
      fprint_boolean(f, 0);
      break;
    case EXPR_INTLIT:
      fprint_integer(f, e -> literal_value);
      break;
    case EXPR_CHARLIT:
      fprintf(f, "'");
      fprint_character(f, e -> char_literal);
      fprintf(f, "'");
      break;
    case EXPR_STRLIT:
      fprintf(f, "\"");
      fprint_string(f, e -> string_literal);
      fprintf(f, "\"");
      break;
    case EXPR_IDENT:
      fprint_string(f, e -> name);
      break;
  }
  /*print expr_lists*/
  if(e -> next) {
    fprintf(f, ", ");
    expr_fprint(f, e -> next);
  }
}

void expr_free(struct expr *e) {
  if(!e) return;
  expr_free(e -> left);
  expr_free(e -> right);
  expr_free(e -> next);
  expr_free(e -> arr_next);
  expr_free(e -> next_list);
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
  expr_resolve(e -> arr_next);
  if(e -> kind == EXPR_IDENT) {
    struct symbol *s = scope_lookup(e -> name);
    if(s) {
      e -> symbol = s;
      if(s -> kind == SYMBOL_GLOBAL) {
        printf("%s resolves to global\n", e -> name);
      }
      else {
        printf("%s resolves to %s %d\n", e -> name, symbol_kind_string(s -> kind), s -> which);
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
  expr_fprint(f, e);
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
    type_fprint(stderr, left);
    fprintf(stderr, " with ");
    type_fprint(stderr, right);
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
    type_fprint(stderr, left);
    fprintf(stderr, " and ");
    type_fprint(stderr, right);
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
      type_fprint(stderr, right);
      expr_typecheck_err_print(stderr, e);
      type_error_count++;
    }
    type_delete(right);
    return type_create(TYPE_BOOLEAN, 0, 0, 0);
  }
  left = expr_typecheck(e -> left);
  if(left -> kind != TYPE_BOOLEAN || right -> kind != TYPE_BOOLEAN) {
    fprintf(stderr, "TYPE_ERROR: cannot apply %s on ", boolop_type);
    type_fprint(stderr, left);
    fprintf(stderr, " and ");
    type_fprint(stderr, right);
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
  type_delete(left);
  type_delete(right);
  return type_create(TYPE_BOOLEAN, 0, 0, 0);
}

int expr_is_constant(struct expr *e) {
  // check to see if there is a next expression
  // if so, array initializer lists typechecking
  // will handle it
  if(e -> next) {
    return 1;
  }
  else if(e -> kind == EXPR_TRUE || e -> kind == EXPR_FALSE || e -> kind == EXPR_INTLIT || e -> kind == EXPR_CHARLIT || e -> kind == EXPR_STRLIT || e -> kind == EXPR_IDENT) {
    if(e -> kind == EXPR_IDENT) {
      struct type *t = e -> symbol -> type;
      // can just check if type is boolean, character,
      // integer, or string because the name has already
      // been resolved at this point and decl_typecheck
      // has previously checked that the declaration is
      // a constant (will print an error if it is not)
      if(t -> kind != TYPE_BOOLEAN || t -> kind != TYPE_CHARACTER || t -> kind != TYPE_INTEGER || t -> kind != TYPE_STRING)
        return 0;
    }
    return 1;
  }
  return 0;
}

int expr_is_num_constant(struct expr *e) {
  return expr_is_constant(e) && (e -> kind == EXPR_INTLIT);
}

// checks if number of indices into array
// is correct; call with symbol -> type
// of array ident and array expression root
struct type *expr_arr_indexcheck(struct type *t, struct expr *e) {
  struct type *curr_type = t;
  struct expr *a_next = e -> arr_next;
  struct type *exp_type = 0;

  while(a_next) {
    // means that there were no more nested
    // subtypes in the declaration
    if(!curr_type) {
      fprintf(stderr, "TYPE_ERROR: there are fewer nested arrays (");
      type_fprint(stderr, t);
      fprintf(stderr, ") in the declaration than the assignment (");
      expr_fprint(stderr, e);
      fprintf(stderr, ") gives\n");
      type_error_count++;
      return 0;
    }

    // if not an integer expr
    exp_type = expr_typecheck(a_next);
    if(exp_type -> kind != TYPE_INTEGER) {
      fprintf(stderr, "TYPE_ERROR: index into an array must be of type integer. Current expr (");
      expr_fprint(stderr, a_next);
      fprintf(stderr, ") is of type ");
      type_fprint(stderr, exp_type);
      fprintf(stderr, "\n");
      type_error_count++;
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
    if(left -> kind != TYPE_ARRAY && left -> kind != TYPE_ARRAY_DECL) {
      fprintf(stderr, "TYPE_ERROR: cannot use array assignment on non-array type ");
      type_fprint(stderr, left);
      expr_typecheck_err_print(stderr, e);
      type_error_count++;
    }
    else {
      // just return the type of the expr
      arr_next = expr_arr_indexcheck(e -> left -> symbol -> type, e);
      if(!arr_next) {
        return right;
      }
      if(!type_compare(arr_next, right)) {
        fprintf(stderr, "TYPE_ERROR: value of index into array (");
        type_fprint(stderr, arr_next);
        fprintf(stderr, ") does not match the type of the expression (");
        type_fprint(stderr, right);
        fprintf(stderr, ")\n");
        type_error_count++;
      }
    }
    type_delete(left);
    return right;
  }
  else {
    if(left -> kind == TYPE_FUNCTION) {
      fprintf(stderr, "TYPE_ERROR: cannot use assignment operator on a function (%s)\n", e -> left -> name);
      type_error_count++;
      type_delete(left);
      return right;
    }
    else {
      if(!type_compare(e -> left -> symbol -> type, right)) {
        fprintf(stderr, "TYPE_ERROR: type value of right assignment (");
        type_fprint(stderr, right);
        fprintf(stderr, ") does not match declared value of ");
        expr_fprint(stderr, e -> left);
        fprintf(stderr, " (");
        type_fprint(stderr, e -> left -> symbol -> type);
        fprintf(stderr, ")\n");
        type_error_count++;
      }
      type_delete(right);
      return left;
    }
  }
}

// takes a base type (should be atomic) and expr_list
// and checks to see if array initalizer (in
// form of expr_list) matches type declared
void expr_arr_init_typecheck(struct expr *name, struct expr *e, struct type *t, struct type *base, int count) {
  if(!e) return;
  // check that the lengths of the expr match

  // go to leaf nested init list, check that
  // e -> left exists beforehand because of
  // possible null ptr dereferencing with
  // t -> subtype
  if(e -> left) {
    // add one more for base atomic subtype
    if(!t -> subtype -> subtype) {
      fprintf(stderr, "TYPE_ERROR: initializer list for ");
      expr_fprint(stderr, name);
      fprintf(stderr, " has more subarrays (");
      expr_fprint(stderr, e);
      fprintf(stderr, ") than declared\n");
      type_error_count++;
      // stop typechecking
      return;
    }
    int newCount = t -> subtype -> expr -> literal_value;
    if(newCount <= 0) {
      fprintf(stderr, "TYPE_ERROR: declared size of array ");
      expr_fprint(stderr, name);
      fprintf(stderr, " is not a positive number (%d)\n", count);
      type_error_count++;
      return;
    }
    // give newCount to nested array (based on
    // the nested type constant size declaration
    expr_arr_init_typecheck(name, e -> left, t -> subtype, base, newCount);
  }
  // if !e -> left is implicit in the else
  // add one more for base atomic subtype
  else if(t -> subtype -> subtype) {
    fprintf(stderr, "TYPE_ERROR: initializer list for ");
    expr_fprint(stderr, name);
    fprintf(stderr, " has fewer subarrays (");
    expr_fprint(stderr, e);
    fprintf(stderr, ") than declared\n");
    type_error_count++;
    // stop typechecking
    return;
  }

  // type compare expr_typecheck result of
  // expr_list with base
  if(!e -> left && !e -> right) {
    struct type *curr_type = expr_typecheck(e);
    if(!type_compare(curr_type, base)) {
      fprintf(stderr, "TYPE_ERROR: initializer expression for ");
      expr_fprint(stderr, name);
      fprintf(stderr, " (");
      expr_fprint(stderr, e);
      fprintf(stderr, ") does not match base type for the array (");
      type_print(base);
      fprintf(stderr, ")\n");
      type_error_count++;
    }
    type_delete(curr_type);
  }
  // not a leaf expr node
  else if(!e -> left) {
    expr_arr_init_typecheck(name, e -> right, t, base, count--);
  }


   /*
    *The below sees if the initializer list
    *exceeds/doesn't meet the declared bounds
    */

  // initializer list has more elements than
  // were declared. this will never get called
  // however, should exist as a check
  if(count < 0) {
    fprintf(stderr, "TYPE_ERROR: initializer list for ");
    expr_fprint(stderr, name);
    fprintf(stderr, " has more elements than were declared\n");
    type_error_count++;
    // don't need to return as it skips rest
    // of code
  }
  // count >= 0 and e -> next_list exists
  else if(e -> next_list) {
    // more array init lists than declared
    if(count == 0) {
      fprintf(stderr, "TYPE_ERROR: initializer list for ");
      expr_fprint(stderr, name);
      fprintf(stderr, " has more elements than were declared\n");
      type_error_count++;
      // don't need to return
    }
    expr_arr_init_typecheck(name, e -> next_list, t, base, count--);
  }
  // implicit !e -> next_list
  // init list has less elems than declared
  else if(count > 0) {
    fprintf(stderr, "TYPE_ERROR: initializer list for ");
    expr_fprint(stderr, name);
    fprintf(stderr, " has fewer elements than were declared\n");
    type_error_count++;
    // don't need to return
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
          type_fprint(stderr, right);
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
      if(e -> left -> kind != EXPR_IDENT) {
        fprintf(stderr, "TYPE_ERROR: increment must be used on an identifier");
        expr_typecheck_err_print(stderr, e);
        type_error_count++;
      }
      // already checked that it is an ident
      else if(left -> kind != TYPE_INTEGER) {
        fprintf(stderr, "TYPE_ERROR: cannot use postincrement on a ");
        type_fprint(stderr, left);
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
      if(e -> left -> kind != EXPR_IDENT) {
        fprintf(stderr, "TYPE_ERROR: increment must be used on an identifier");
        expr_typecheck_err_print(stderr, e);
        type_error_count++;
      }
      // already checked that it is an ident
      else if(left -> kind != TYPE_INTEGER) {
        fprintf(stderr, "TYPE_ERROR: cannot use postdecrement on a ");
        type_fprint(stderr, left);
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
      if((e -> left -> symbol -> type -> kind != TYPE_ARRAY) && (e -> left -> symbol -> type -> kind != TYPE_ARRAY_DECL)) {
        fprintf(stderr, "TYPE_ERROR: the identifier (");
        expr_fprint(stderr, e -> left);
        fprintf(stderr, ") is not of type array\n");
        type_error_count++;
        // don't know what else to return, so
        // return an integer
        return type_create(TYPE_INTEGER, 0, 0, 0);
      }
      left = expr_arr_indexcheck(e -> left -> symbol -> type, e);
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
    case EXPR_ARR_INITLIST:
      fprintf(stderr, "Bad call on expr_typecheck (EXPR_ARR_INITLIST should only be called by special case in decl_typcheck)\n");
      // program (not user) error
      exit(1);
      /*return expr_arr_init_typecheck(e);*/
    case EXPR_GROUP:
      return expr_typecheck(e -> right);
    case EXPR_FUNC:
      if(e -> left -> symbol -> type -> kind != TYPE_FUNCTION) {
        fprintf(stderr, "TYPE_ERROR: identifier (%s) for function call is not a function\n", e -> left -> name);
        type_error_count++;
        // return the type of the identifier
        return type_copy(e -> left -> symbol -> type);
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

void expr_assembly_op_comment(struct expr *e, FILE *f, const char *op) {
  if(ASSEMBLY_COMMENT_FLAG) {
    fprintf(f, "\n\t# ");
    expr_fprint(f, e -> left);
    fprintf(f, " %s ", op);
    expr_fprint(f, e -> right);
    fprintf(f, "\n");
  }
}

void expr_assembly_lit_comment(FILE *f, char *assign_val) {
  if(ASSEMBLY_COMMENT_FLAG) {
    fprintf(f, "\n\t# Move %s into register\n", assign_val);
  }
}

// add and subtract
void expr_add_codegen(struct expr *e, FILE *f, int which) {
  const char *op;
  if(which == 0) {
    op = "ADDQ";
  }
  else if(which == 1) {
    op = "SUBQ";
  }
  else {
    fprintf(stderr, "Error in calling function expr_add_codegen (must be of expr type PLUS or MIN\n");
    exit(1);
  }
  expr_codegen(e -> left, f);
  expr_codegen(e -> right, f);

  expr_assembly_op_comment(e, f, op);
  fprintf(f, "\t%s %s, %s\n", op, register_name(e -> left -> reg), register_name(e -> right -> reg));
  e -> reg = e -> right -> reg;
  register_free(e -> left -> reg);
}

// divide and modulo
void expr_div_codegen(struct expr *e, FILE *f, int which) {
  const char *op, *reg;
  if(which == 0) {
    op = "DIV";
    reg = "%%rax";
  }
  else if(which == 1) {
    op = "MOD";
    reg = "%%rdx";
  }
  else {
    fprintf(stderr, "Error in calling function expr_div_codegen (must be of expr type DIV or MOD\n");
    exit(1);
  }
  expr_codegen(e -> left, f);
  expr_codegen(e -> right, f);
  expr_assembly_op_comment(e, f, op);

  // the below moves value of rdx into another register
  // (this is because rdx is overwritten by the IDIV instr)
  int tmpReg = register_alloc();
  fprintf(f, "\tMOVQ %%rdx, %s\n", register_name(tmpReg));
  fprintf(f, "\tMOVQ %s, %%rax\n", register_name(e -> left -> reg));
  fprintf(f, "\tCDQO\n");
  fprintf(f, "\tIDIVQ %s\n", register_name(e -> right -> reg));
  fprintf(f, "\tMOVQ %s, %s\n", reg, register_name(e -> right -> reg));
  fprintf(f, "\tMOVQ %s, %%rdx\n", register_name(tmpReg));
  register_free(tmpReg);
  e -> reg = e -> right -> reg;
  register_free(e -> left -> reg);
}

// postincrement and postdecrement
void expr_post_codegen(struct expr *e, FILE *f, int which) {
  const char *op;
  if(which == 0) {
    op = "INCQ";
  }
  else if(which == 1) {
    op = "DECQ";
  }
  else {
    fprintf(stderr, "Error in calling function expr_post_increment (must be of expr type PLUSPLUS or MINMIN\n");
    exit(1);
  }
  expr_codegen(e -> left, f);
  // has already been done typechecked but still call it to see
  // if any errors occur
  expr_codegen(e -> right, f);
  expr_assembly_op_comment(e, f, op);
  fprintf(f, "\tMOVQ %s, %%rax\n", register_name(e -> left -> reg));
  fprintf(f, "\t%s %%rax\n", op);
  fprintf(f, "\tMOVQ %%rax, %s\n", register_name(e -> left -> reg));
  e -> reg = e -> left -> reg;
  // will cause errors with reg_name[0]
  /*register_free(e -> right -> reg);*/
}

// places func result in e -> reg, pass in func name
void expr_func_codegen(struct expr *e, const char *name, FILE *f) {
  assembly_comment(f, "\t# save caller-saved registers\n");
  fprintf(f, "\tPUSHQ %%r10\n");
  fprintf(f, "\tPUSHQ %%r11\n");
  assembly_comment(f, "\t# call actual function\n");
#ifdef __linux__
  fprintf(f, "\tCALL %s\n", name);
#elif __APPLE__
  fprintf(f, "\tCALL _%s\n", name);
#else
  fprintf(f, "\tCALL %s\n", name);
#endif
  assembly_comment(f, "\t# unsave caller-saved registers\n");
  fprintf(f, "\tPOPQ %%r11\n");
  fprintf(f, "\tPOPQ %%r10\n");
  e -> reg = register_alloc();
  assembly_comment(f, "\t# save function return value\n");
  fprintf(f, "\tMOVQ %%rax, %s\n", register_name(e -> reg));
}

void expr_comp_codegen(struct expr *e, FILE *f, int which) {
  char *comp_type = 0;
  char *op = 0;
  switch(which) {
    case 0:
      comp_type = "less than";
      op = "JL";
      break;
    case 1:
      comp_type = "less than or equal";
      op = "JLE";
      break;
    case 2:
      comp_type = "greater than";
      op = "JG";
      break;
    case 3:
      comp_type = "greater than or equal";
      op = "JGE";
      break;
    case 4:
      comp_type = "equal equal";
      op = "JE";
      break;
    case 5:
      comp_type = "not equal";
      op = "JNE";
      break;
  }
  expr_codegen(e -> left, f);
  expr_codegen(e -> right, f);
  int tr = assembly_jump_label++;
  int done = assembly_jump_label++;

  expr_assembly_op_comment(e, f, comp_type);

  fprintf(f, "\tCMP %s, %s\n", register_name(e -> left -> reg), register_name(e -> right -> reg));
  // jump to boolean result of true
  fprintf(f, "\t%s L%d\n", op, tr);

  // boolean false
  fprintf(f, "\tMOVQ $0, %s\n", register_name(e -> right -> reg));
  assembly_comment(f, "\t# jump to done label (don't evaluate set to true)\n");
  fprintf(f, "\tJMP L%d\n", done);

  assembly_comment(f, "\t# true label\n");
  fprintf(f, "L%d:\n", tr);
  fprintf(f, "\tMOVQ $1, %s\n", register_name(e -> right -> reg));

  assembly_comment(f, "\t# done label\n");
  fprintf(f, "L%d:\n", done);

  e -> reg = e -> right -> reg;
  register_free(e -> left -> reg);
}

void expr_comp_string_codegen(struct expr *e, FILE *f) {
  expr_codegen(e -> left, f);
  expr_codegen(e -> right, f);

  int count = 0;
  // put arg 0 (left) of expr in arg reg 0
  if(ASSEMBLY_COMMENT_FLAG) {
    fprintf(f, "\t# move arg %d (in %s) into %s", count, register_name(e -> left -> reg), register_arg_names[count]);
  }
  fprintf(f, "MOVQ %s, %s\n", register_name(e -> left -> reg), register_arg_names[count++]);

  // put arg 1 (right) of expr in arg reg 1
  if(ASSEMBLY_COMMENT_FLAG) {
    fprintf(f, "\t# move arg %d (in %s) into %s\n", count, register_name(e -> right -> reg), register_arg_names[count]);
  }
  fprintf(f, "\tMOVQ %s, %s\n", register_name(e -> right -> reg), register_arg_names[count++]);
  register_free(e -> left -> reg);
  register_free(e -> right -> reg);
  expr_func_codegen(e, "strcmp", f);
}

void expr_codegen(struct expr *e, FILE *f) {
  if(!e) return;
  char *val;
  switch(e -> kind) {
    case EXPR_PLUS:
      expr_add_codegen(e, f, 0);
      break;
    case EXPR_MIN:
      expr_add_codegen(e, f, 1);
      break;
    case EXPR_MUL:
      expr_codegen(e -> left, f);
      expr_codegen(e -> right, f);
      expr_assembly_op_comment(e, f, "multiply");
      fprintf(f, "\tMOVQ %s, %%rax\n", register_name(e -> left -> reg));
      fprintf(f, "\tIMULQ %s\n", register_name(e -> right -> reg));
      fprintf(f, "\tMOVQ %%rax, %s\n", register_name(e -> right -> reg));
      e -> reg = e -> right -> reg;
      register_free(e -> left -> reg);
      break;
    case EXPR_DIV:
      expr_div_codegen(e, f, 0);
      break;
    case EXPR_MOD:
      expr_div_codegen(e, f, 1);
      break;
    case EXPR_PLUSPLUS:
      expr_post_codegen(e, f, 0);
      break;
    case EXPR_MINMIN:
      expr_post_codegen(e, f, 1);
      break;
    case EXPR_EXP: {
      expr_codegen(e -> left, f);
      expr_codegen(e -> right, f);

      int count = 0;
      // put arg 0 (left) of expr in arg reg 0
      if(ASSEMBLY_COMMENT_FLAG) {
        fprintf(f, "\t# move arg %d (in %s) into %s", count, register_name(e -> left -> reg), register_arg_names[count]);
      }
      fprintf(f, "MOVQ %s, %s\n", register_name(e -> left -> reg), register_arg_names[count++]);

      // put arg 1 (right) of expr in arg reg 1
      if(ASSEMBLY_COMMENT_FLAG) {
        fprintf(f, "\t# move arg %d (in %s) into %s\n", count, register_name(e -> right -> reg), register_arg_names[count]);
      }
      fprintf(f, "\tMOVQ %s, %s\n", register_name(e -> right -> reg), register_arg_names[count++]);
      register_free(e -> left -> reg);
      register_free(e -> right -> reg);
      expr_func_codegen(e, "integer_power", f);
      break;
    }
    case EXPR_LT:
      expr_comp_codegen(e, f, 0);
      break;
    case EXPR_LE:
      expr_comp_codegen(e, f, 1);
      break;
    case EXPR_GT:
      expr_comp_codegen(e, f, 2);
      break;
    case EXPR_GE:
      expr_comp_codegen(e, f, 3);
      break;
    case EXPR_EQEQ:
      // check that left is str lit or that it
      // is an ident of type str (already typechecked)
      if(e -> left -> kind == EXPR_STRLIT || e -> left -> symbol -> type -> kind == TYPE_STRING) {
        expr_comp_string_codegen(e, f);

        int tr = assembly_jump_label++;
        int done = assembly_jump_label++;

        assembly_comment(f, "\t# compare result of strcmp to 0 (-1/1 mean str 1 is lexicographically lt/gt str 2)\n");
        fprintf(f, "\tCMP $0, %s\n", register_name(e -> reg));
        // jump to boolean result of true
        fprintf(f, "\tJE L%d\n", tr);

        // boolean false
        fprintf(f, "\tMOVQ $0, %s\n", register_name(e -> reg));
        assembly_comment(f, "\t# jump to done label (don't evaluate set to true)\n");
        fprintf(f, "\tJMP L%d\n", done);

        assembly_comment(f, "\t# true label\n"); fprintf(f, "L%d:\n", tr);
        fprintf(f, "\tMOVQ $1, %s\n", register_name(e -> reg));

        assembly_comment(f, "\t# done label\n");
        fprintf(f, "L%d:\n", done);
      }
      // other equality checks
      else {
        expr_comp_codegen(e, f, 4);
      }
      break;
    case EXPR_NE:

      break;
    case EXPR_AND: {
      expr_codegen(e -> left, f);
      expr_codegen(e -> right, f);
      expr_assembly_op_comment(e, f, "and");

      int tr = assembly_jump_label++;
      int fal = assembly_jump_label++;
      int done = assembly_jump_label++;

      // implement short circuiting
      assembly_comment(f, "\t# check that left expr is false\n");
      fprintf(f, "\tCMP $0, %s\n", register_name(e -> left -> reg));
      // jump to boolean result of false
      fprintf(f, "\tJE L%d\n", fal);

      assembly_comment(f, "\t# check that right expr is false\n");
      fprintf(f, "\tCMP $0, %s\n", register_name(e -> right -> reg));
      // jump to boolean result of true
      fprintf(f, "\tJNE L%d\n", tr);

      // boolean false
      assembly_comment(f, "\t# false label\n");
      fprintf(f, "L%d:\n", fal);
      fprintf(f, "\tMOVQ $0, %s\n", register_name(e -> right -> reg));
      assembly_comment(f, "\t# jump to done label (don't evaluate set to true)\n");
      fprintf(f, "\tJMP L%d\n", done);

      assembly_comment(f, "\t# true label\n");
      fprintf(f, "L%d:\n", tr);
      fprintf(f, "\tMOVQ $1, %s\n", register_name(e -> right -> reg));

      assembly_comment(f, "\t# done label\n");
      fprintf(f, "L%d:\n", done);

      e -> reg = e -> right -> reg;
      register_free(e -> left -> reg);
      break;
    }
    case EXPR_OR:
      expr_codegen(e -> left, f);
      expr_codegen(e -> right, f);
      expr_assembly_op_comment(e, f, "or");

      int tr = assembly_jump_label++;
      int fal = assembly_jump_label++;
      int done = assembly_jump_label++;

      // implement short circuiting
      assembly_comment(f, "\t# check that left expr is true\n");
      fprintf(f, "\tCMP $1, %s\n", register_name(e -> left -> reg));
      // jump to boolean result of true
      fprintf(f, "\tJE L%d\n", tr);

      assembly_comment(f, "\t# check that right expr is true\n");
      fprintf(f, "\tCMP $1, %s\n", register_name(e -> right -> reg));
      // jump to boolean result of false
      fprintf(f, "\tJNE L%d\n", fal);

      // boolean false
      assembly_comment(f, "\t# true label\n");
      fprintf(f, "L%d:\n", tr);
      fprintf(f, "\tMOVQ $1, %s\n", register_name(e -> right -> reg));
      assembly_comment(f, "\t# jump to done label (don't evaluate set to false)\n");
      fprintf(f, "\tJMP L%d\n", done);

      assembly_comment(f, "\t# false label\n");
      fprintf(f, "L%d:\n", fal);
      fprintf(f, "\tMOVQ $1, %s\n", register_name(e -> right -> reg));

      assembly_comment(f, "\t# done label\n");
      fprintf(f, "L%d:\n", done);

      e -> reg = e -> right -> reg;
      register_free(e -> left -> reg);
      break;
    case EXPR_NOT:
      expr_codegen(e -> right, f);
      expr_assembly_op_comment(e, f, "not");
      fprintf(f, "\tSUBQ $1, %s\n", register_name(e -> right -> reg));
      fprintf(f, "\tSBB %s, %s\n", register_name(e -> right -> reg), register_name(e -> right -> reg));
      fprintf(f, "\tAND $1, %s\n", register_name(e -> right -> reg));
      e -> reg = e -> right -> reg;
      break;
    case EXPR_EQ:
      if(e -> left -> symbol -> type -> kind == TYPE_ARRAY_DECL || e -> left -> symbol -> type -> kind == TYPE_ARRAY) {
        fprintf(stderr, "CODEGEN_ERROR: cminor does not have an array implementation\n");
        exit(1);
      }
      expr_codegen(e -> right, f);
      expr_assembly_op_comment(e, f, "assign");
      fprintf(f, "\tMOV %s, %s", register_name(e -> right -> reg), symbol_code(e -> left -> symbol));
      e -> reg = e -> right -> reg;
      break;
    case EXPR_ARREQ:
      fprintf(stderr, "CODEGEN_ERROR: cminor does not have an array implementation\n");
      exit(1);
      break;
    case EXPR_ARR:
      fprintf(stderr, "CODEGEN_ERROR: cminor does not have an array implementation\n");
      exit(1);
      break;
    case EXPR_ARR_INITLIST:
      fprintf(stderr, "CODEGEN_ERROR: cminor does not have an array implementation\n");
      exit(1);
      break;
    case EXPR_GROUP:
      expr_codegen(e -> right, f);
      e -> reg = e -> right -> reg;
      break;
    case EXPR_FUNC:
      assembly_mov_arg_registers(f, e -> right);
      expr_func_codegen(e, e -> left -> name, f);
      break;
    case EXPR_TRUE:
      e -> reg = register_alloc();
      expr_assembly_lit_comment(f, "false");
      fprintf(f, "\tMOVQ $1, %s\n", register_name(e -> reg));
      break;
    case EXPR_FALSE:
      e -> reg = register_alloc();
      expr_assembly_lit_comment(f, "false");
      fprintf(f, "\tMOVQ $0, %s\n", register_name(e -> reg));
      break;
    case EXPR_INTLIT:
      e -> reg = register_alloc();
      asprintf(&val, "%d", e -> literal_value);
      expr_assembly_lit_comment(f, val);
      free(val);
      fprintf(f, "\tMOVQ $%d, %s\n", e -> literal_value, register_name(e -> reg));
      break;
    case EXPR_CHARLIT:
      e -> reg = register_alloc();
      asprintf(&val, "%c", e -> char_literal);
      expr_assembly_lit_comment(f, val);
      free(val);
      fprintf(f, "\tMOVQ $%d, %s\n", e -> char_literal, register_name(e -> reg));
      break;
    case EXPR_STRLIT:
      e -> reg = register_alloc();
      if(e -> str_num == -1) {
        // set this string constant to current global
        e -> str_num = expr_num_str;
        fprintf(f, ".data\n");
        fprintf(f, "STR%d:\n", expr_num_str++);
        val = assembly_string_out((char *) e -> string_literal);
        fprintf(f, "\t.string \"%s\"\n", val);
        free(val);
        fprintf(f, ".text\n");
      }
      if(ASSEMBLY_COMMENT_FLAG) {
        fprintf(f, "\t# move STR%d into register\n", e -> str_num);
      }
      // put the string in reg
      fprintf(f, "\tMOV STR%d, %s\n", e -> str_num, register_name(e -> reg));
      break;
    case EXPR_IDENT:
      if(e -> symbol -> type -> kind == TYPE_STRING) {
        e -> reg = register_alloc();
        int str_num = e -> symbol -> orig_decl -> value -> str_num;
        if(ASSEMBLY_COMMENT_FLAG) {
          // the str_num is stored in the original decl
          fprintf(f, "\t# move STR%d into register\n", str_num);
        }
        // put the string in reg
        fprintf(f, "\tLEA STR%d, %s\n", str_num, register_name(e -> reg));
      }
      else {
        e -> reg = register_alloc();
        val = symbol_code(e -> symbol);
        if(ASSEMBLY_COMMENT_FLAG) {
          fprintf(f, "\t# move %s %d into %s\n", symbol_kind_string(e -> symbol -> kind), e -> symbol -> which, register_name(e -> reg));
        }
        fprintf(f, "\tMOVQ %s, %s\n", val, register_name(e -> reg));
        free(val);
      }
      break;
  }
}

