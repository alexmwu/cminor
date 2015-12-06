#include "assembly.h"

void assembly_comment(FILE *f, const char *s) {
  if(ASSEMBLY_COMMENT_FLAG) {
    fprintf(f, "%s", s);
  }
}

void assembly_codegen(struct decl *d, FILE *f) {
  fprintf(f, ".text\n");
  fprintf(f, ".p2align4\n");
  decl_codegen(d, f, SYMBOL_GLOBAL);
}

