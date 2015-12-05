#include "assembly.h"

void assembly_comment(FILE *f, const char *s) {
  if(ASSEMBLY_COMMENT_FLAG) {
    fprintf(f, "%s", s);
  }
}

void assembly_codegen(struct decl *d, FILE *f) {
}

