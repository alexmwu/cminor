#ifndef REGISTER_H
#define REGISTER_H

#define NUM_REGISTERS 7

#ifdef _linux_
  ASM_ENV = 1
#elif _APPLE_
  ASM_ENV = 2
#else
  ASM_ENV = 0
#endif

static const char *register_name_string[NUM_REGISTERS] = {"rbx", "r10", "r11", "r12", "r13", "r14", "r15"};
static int register_is_free[NUM_REGISTERS] = {1, 1, 1, 1, 1, 1, 1};

const char *register_name(int r);
// mark register as in use
int register_alloc();
// remove register in use
void register_free(int r);

#endif

