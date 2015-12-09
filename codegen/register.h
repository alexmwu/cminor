#ifndef REGISTER_H
#define REGISTER_H

#define REGISTER_NUM 7

static const char *register_name_string[REGISTER_NUM] = {"%rbx", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15"};

#define REGISTER_NUM_ARG 6

static const char *register_arg_names[REGISTER_NUM_ARG] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};

const char *register_name(int r);
// mark register as in use
int register_alloc();
// remove register in use
void register_free(int r);

#endif

