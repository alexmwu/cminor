#ifndef EXPR_H
#define EXPR_H

#include <stdio.h>
#include "symbol.h"
#include "scope.h"

typedef enum {
	EXPR_PLUS,
	EXPR_MIN,
	EXPR_MUL,
	EXPR_DIV,
  EXPR_MOD,
  EXPR_PLUSPLUS,
  EXPR_MINMIN,
  EXPR_EXP,
  EXPR_LT,
  EXPR_LE,
  EXPR_GT,
  EXPR_GE,
  EXPR_EQEQ,
  EXPR_NE,
  EXPR_AND,
  EXPR_OR,
  EXPR_NOT,
  EXPR_EQ,
  EXPR_ARREQ,
  EXPR_ARR,
  EXPR_ARR_INITLIST,
  EXPR_GROUP,
  EXPR_FUNC,
  EXPR_TRUE,
  EXPR_FALSE,
  EXPR_INTLIT,
  EXPR_CHARLIT,
  EXPR_STRLIT,
  EXPR_IDENT
} expr_t;

struct expr {
	/* used by most expr types */
	expr_t kind;
	struct expr *left;
	struct expr *right;

  // used by expr_lists
  struct expr *next;
  // used by array indices
  struct expr *arr_next;
  // used by array initializer lists
  struct expr *next_list;

	/* used by leaf expr types */
	const char *name;
	struct symbol *symbol;
	int literal_value;
	const char * string_literal;
  // global string number (for codegen)
  int str_num;
  char char_literal;

  // register holding the expr value
  int reg;
  // print type for atomics
  // 0 for undef; 1 for bool, 2 char, 3 int, 4 str
  int print_type;
};

// number of strings declared
int expr_num_str;

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right, struct expr *next );

struct expr * expr_create_name( const char *n );
struct expr * expr_create_boolean_literal( int c );
struct expr * expr_create_integer_literal( int c );
struct expr * expr_create_character_literal( char c );
struct expr * expr_create_string_literal( const char *str );

void expr_print( struct expr *e );
void expr_fprint(FILE *f, struct expr *e);

void expr_free(struct expr *e);

void expr_resolve(struct expr *e);

int expr_is_constant(struct expr *e);
int expr_is_num_constant(struct expr *e);
void expr_arr_init_typecheck(struct expr *name, struct expr *e, struct type *t, struct type *base, int count);
void expr_typecheck_err_print(FILE *f, struct expr *e);
struct type *expr_typecheck(struct expr *e);

void expr_func_codegen(struct expr *e, const char *name, FILE *f);
void expr_codegen(struct expr *e, FILE *f);

#endif

