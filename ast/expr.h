#ifndef EXPR_H
#define EXPR_H

#include "symbol.h"

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

	/* used by leaf expr types */
	const char *name;
	struct symbol *symbol;
	int literal_value;
	const char * string_literal;
  char char_literal;
};

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right, struct expr *next );

struct expr * expr_create_name( const char *n );
struct expr * expr_create_boolean_literal( int c );
struct expr * expr_create_integer_literal( int c );
struct expr * expr_create_character_literal( char c );
struct expr * expr_create_string_literal( const char *str );

void expr_print( struct expr *e );
void expr_free(struct expr *e);

#endif
