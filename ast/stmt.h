
#ifndef STMT_H
#define STMT_H

#include "decl.h"
#include <stdio.h>

typedef enum {
	STMT_DECL,
	STMT_EXPR,
	STMT_IF_ELSE,
	STMT_FOR,
	STMT_WHILE,
	STMT_PRINT,
	STMT_RET,
	STMT_BLOCK
} stmt_kind_t;

struct stmt {
	stmt_kind_t kind;
	struct decl *decl;
	struct expr *init_expr;
	struct expr *expr;
	struct expr *next_expr;
	struct stmt *body;
	struct stmt *else_body;
	struct stmt *next;
};

struct stmt * stmt_create( stmt_kind_t kind, struct decl *d, struct expr *init_expr, struct expr *e, struct expr *next_expr, struct stmt *body, struct stmt *else_body );

void stmt_print( struct stmt *s, int indent );
void stmt_fprint(FILE *f, struct stmt *s, int indent);

void stmt_free(struct stmt *s);

void stmt_resolve(struct stmt *s, int *which);

// ret and returned are for function
// declarations. ret is the type
// that the function should return
// returned is whether there was
// a return statement
void stmt_typecheck(struct stmt *s, struct type *ret, int *returned);

void stmt_codegen(struct stmt *s, FILE *f);

#endif

