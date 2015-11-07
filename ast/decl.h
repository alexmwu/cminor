
#ifndef DECL_H
#define DECL_H

#include "type.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "library.h"
#include <stdio.h>

struct decl {
	struct expr *name;
	struct type *type;
	struct expr *value;
	struct stmt *code;
	struct symbol *symbol;
	struct decl *next;
};

struct decl * decl_create( struct expr *name, struct type *t, struct expr *v, struct stmt *c, struct decl *next );
void decl_print( struct decl *d, int indent );
void decl_free(struct decl *d);

#endif


