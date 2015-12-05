
#ifndef PARAM_LIST_H
#define PARAM_LIST_H

#include "type.h"
#include <stdio.h>

struct expr;

struct param_list {
	struct expr *name;
	struct type *type;
	struct symbol *symbol;
	struct param_list *next;
};

struct param_list * param_list_create( struct expr *name, struct type *type, struct param_list *next );
void param_list_print( struct param_list *a );
void param_list_fprint(FILE *f, struct param_list *a);
void param_list_free(struct param_list *a);
void param_list_resolve(struct param_list *a, int which);
void param_list_typecheck(struct param_list *p_list, struct expr *exp_list, const char *name);

#endif
