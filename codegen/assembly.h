#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include "../ast/decl.h"
#include <stdio.h>

#ifdef _linux_
#elif _APPLE_
#else
  fprintf(stderr, "Non-linux or OSX platform. Other platforms (including Windows) are unsupported\n");
  exit(1);
#endif

int ASSEMBLY_COMMENT_FLAG = 0;
void assembly_comment(FILE *f, const char *s);

// entry point for codegen
// this prints out boilerplate
void assembly_codegen(struct decl *d, FILE *f);

#endif

