#include "register.h"
#include <stdlib.h>
#include <stdio.h>

const char *register_name(int r) {
  return register_name_string[r];
}

int register_alloc() {
  int i;
  for(i = 0; i < REGISTER_NUM; i++) {
    if(register_is_free[i]) {
      register_is_free[i] = 0;
      return i;
    }
  }
  fprintf(stderr, "CODEGEN_ERROR: cminor only supports simple register allocation. Out of scratch registers to allocate.\n");
  exit(1);
}

void register_free(int r) {
  register_is_free[r] = 1;
}

