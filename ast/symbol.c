#include "symbol.h"
#include <stdlib.h>
#include <stdio.h>

struct symbol *symbol_create(symbol_t kind, struct type *type, struct expr *name) {
  struct symbol *s = malloc(sizeof *s);
  s -> kind = kind;
  s -> type = type;
  s -> name = name;
  s -> which = 0;
  return s;
}

void symbol_free(struct symbol *s) {
  if(!s) return;
  type_free(s -> type);
  expr_free(s -> name);
  free(s);
}

const char *symbol_kind_print(symbol_t k) {
  switch(k) {
    case SYMBOL_LOCAL:
      return "local";
    case SYMBOL_PARAM:
      return "param";
    case SYMBOL_GLOBAL:
      return "global";
  }
}

void symbol_print(struct symbol *s) {
  if(!s) return;
  switch(s -> kind) {
    case SYMBOL_LOCAL:
      printf("local %d ", s -> which);
      expr_print(s -> name);
      printf(" of type ");
      type_print(s -> type);
      printf("\n");
      break;
    case SYMBOL_PARAM:
      printf("param %d ", s -> which);
      expr_print(s -> name);
      printf(" of type ");
      type_print(s -> type);
      printf("\n");
      break;
    case SYMBOL_GLOBAL:
      printf("global ");
      expr_print(s -> name);
      printf(" of type ");
      type_print(s -> type);
      printf("\n");
      break;
  }
}

