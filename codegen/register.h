#ifndef REGISTER_H
#define REGISTER_H

#define NUM_REGISTERS 7

static const char *register_name_string[NUM_REGISTERS] = {"%rbx", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15"};
static int register_is_free[NUM_REGISTERS] = {1, 1, 1, 1, 1, 1, 1};

#define NUM_ARG_REGISTERS

static const char *register_arg_names[NUM_ARG_REGISTERS] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};

const char *register_name(int r);
// mark register as in use
int register_alloc();
// remove register in use
void register_free(int r);

#endif

