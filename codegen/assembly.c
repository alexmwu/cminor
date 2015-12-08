#include "assembly.h"
#include "register.h"
#include <string.h>
#include <stdlib.h>

void assembly_comment(FILE *f, const char *s) {
  if(ASSEMBLY_COMMENT_FLAG) {
    fprintf(f, "%s", s);
  }
}

void assembly_codegen(struct decl *d, FILE *f) {
#ifdef __APPLE__
  // minimum supported version
  // of OSX is 10.11
  fprintf(f, ".macosx_version_min 10, 11\n");
  // align stack on a 16-byte boundary
  // (cannot be an odd sum of locals
  // params, and callee saved regs)
  fprintf(f, ".p2align 4\n");
#endif
  fprintf(f, ".text\n");
  decl_codegen(d, f, SYMBOL_GLOBAL);
}

// only used by string literals
char *assembly_string_out(char *s) {
  int len = strlen(s);
  // length (just in case they are all backslashes), plus null
  int newLen = 1 + (len * 2);
  char *retStr = malloc(newLen * sizeof(char));
  char *ptr = s;
  // pointer into retStr
  int i = 0;
  while(ptr) {
    if(*ptr == '"') {
      retStr[i++] = '\\';
      retStr[i++] = '"';
    }
    else if(*ptr == '\'') {
      retStr[i++] = '\\';
      retStr[i++] = '\'';
    }
    else if(*ptr == '\\') {
      retStr[i++] = '\\';
      retStr[i++] = '\\';
    }
    else if(*ptr == '\n') {
      retStr[i++] = '\\';
      retStr[i++] = 'n';
    }
    else {
      retStr[i++] = *ptr;
    }
  }
  retStr[i] = '\0';
  // exceed bounds of malloc
  if(i >= newLen) {
    fprintf(stderr, "assembly_string_out exceeded malloc'd bounds; check implementation\n");
    exit(1);
  }
  char *newStr;
  if(i < newLen - 1) {
    newStr = malloc(i * sizeof(char));
    strcpy(newStr, retStr);
    free(retStr);
    retStr = newStr;
  }
  return retStr;
}

void assembly_arg_stack_alloc(FILE *f, int num_args) {
  if(num_args < 0) {
    fprintf(f, "Called assembly_arg_stack_alloc with negative (%d) number of arguments\n", num_args);
    exit(1);
  }
  else if(num_args > REGISTER_NUM_ARG) {
    fprintf(f, "CODEGEN_ERROR: cminor does not support functions with more than %d arguments\n", REGISTER_NUM_ARG);
    exit(1);
  }
  int i;
  for(i = 0; i < num_args; i++) {
    if(ASSEMBLY_COMMENT_FLAG) {
      fprintf(f, "\t# save arg %d on the stack\n", i + 1);
    }
    fprintf(f, "\tPUSHQ %s\n", register_arg_names[i]);
  }
}

void assembly_mov_arg_registers(FILE *f, struct expr *expr_list) {
  struct expr *curr_expr = expr_list;
  int count = 0;
  while(curr_expr) {
    if(count > REGISTER_NUM_ARG) {
      fprintf(f, "CODEGEN_ERROR: cminor does not support functions with more than %d arguments\n", REGISTER_NUM_ARG);
      exit(1);
    }
    expr_codegen(curr_expr, f);
    if(ASSEMBLY_COMMENT_FLAG) {
      fprintf(f, "\t# move arg %d (in %s) into %s", count, register_name(curr_expr -> reg), register_arg_names[count]);
    }
    fprintf(f, "MOVQ %s, %s\n", register_name(curr_expr -> reg), register_arg_names[count++]);
    register_free(curr_expr -> reg);
    curr_expr = curr_expr -> next;
  }
}

