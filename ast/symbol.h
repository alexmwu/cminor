
#ifndef SYMBOL_H
#define SYMBOL_H

#include "type.h"

typedef enum {
	SYMBOL_LOCAL,
	SYMBOL_PARAM,
	SYMBOL_GLOBAL
} symbol_t;

struct symbol {
	symbol_t kind;
	int which;
	struct type *type;
	struct expr *name;
};

struct symbol * symbol_create( symbol_t kind, struct type *type, struct expr *name );
void symbol_free(struct symbol *s);
const char *symbol_kind_print(symbol_t k);
void symbol_print(struct symbol *s);

#endif
