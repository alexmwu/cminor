#include "register.h"
#include <stdlib.h>
#include <stdio.h>

const char *register_name(int r) {
  return register_name_string[r];
}

int register_alloc() {
  int i;
  for(i = 0; i < NUM_REGISTERS; i++) {
    if(register_is_free[i]) {
      register_is_free[i] = 0;
      return i;
    }
  }
  fprintf(stderr, "Out of scratch registers to allocate");
  exit(1);
}

void register_free(int r) {
  register_is_free[r] = 1;
}

