#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include "../ast/decl.h"
#include <stdio.h>

#ifdef _linux_
  #define ASM_ENV 1
#elif _APPLE_
  #define ASM_ENV 2
#else
  #define ASM_ENV 0
#endif

int ASSEMBLY_COMMENT_FLAG = 0;
void assembly_comment(FILE *f, const char *s);

// entry point for codegen
// this prints out boilerplate
void assembly_codegen(struct decl *d, FILE *f);

#endif

