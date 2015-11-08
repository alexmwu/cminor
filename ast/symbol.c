#include "symbol.h"
#include <stdlib.h>

struct symbol *symbol_create(symbol_t kind, struct type *type, char *name) {
  struct symbol *s = malloc(sizeof s);
  s -> kind = kind;
  s -> type = type;
  s -> name = name;
  return s;
}

void symbol_free(struct symbol *s) {
  if(!s) return;
  type_free(s -> type);
  free(s -> name);
  free(s);
}
