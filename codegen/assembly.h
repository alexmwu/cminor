#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include "../ast/decl.h"
#include <stdio.h>

int ASSEMBLY_COMMENT_FLAG;
void assembly_comment(FILE *f, const char *s);

// entry point for codegen
// this prints out boilerplate
void assembly_codegen(struct decl *d, FILE *f);

// convert null terminator and byte based
// string representation into a string literal in asm
char *assembly_string_out(char *s);

// pass in the number of declared params
void assembly_arg_stack_alloc(FILE *f, int num_args);

#endif

