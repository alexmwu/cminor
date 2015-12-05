#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include "../ast/decl.h"
#include <stdio.h>

int ASSEMBLY_COMMENT_FLAG = 0;
void assembly_comment(FILE *f, const char *s);

// entry point for codegen
// this prints out boilerplate
void assembly_codegen(struct decl *d, FILE *f);

#endif

