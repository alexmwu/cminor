#include <stdio.h>
#include "grammar/token.h"
#include "grammar/handle.h"
#include "scanner.yy.h"
#include "parser.tab.h"
#include "ast/decl.h"
#include "codegen/assembly.h"

extern FILE *yyin;
extern int yyparse();
extern struct decl *programRoot;

// temporary help printout
void printHelp() {
  printf("The CMinor Compiler\n\n");
  printf("usage: cminor [--help|<command>] [filename]\n\n");
  printf("*Filenames are optional; the default is stdin\n");
  printf("*All lower commands make use of steps in previous commands (e.g., -parse uses -scan code)\n");
  printf("These are optional commands for the CMinor compiler:\n");
  printf("\t-scan\tRun an optional file through CMinor scanner\n");
  printf("\t-parse\tRun an optional file through CMinor parser\n");
  printf("\t-resolve\tRun a file through CMinor resolver and see if there are name resolution errors\n");
  printf("\t-typecheck\tRun a file through CMinor typechecker and see if there are type errors\n");
  printf("\t-codegen\tRun a file through CMinor code generator and produce x86_64 assembly code\n");
}

void printGoHelp(char *command) {
  printf("cminor: '%s' is not a cminor command. See 'cminor --help'.\n", command);
}

int main(int argc, char **argv) {
  yyin = stdin;
  // TODO: this is a bit of a hack to make printfs print in
  // sequence with fprintf (rather than flushing only on newlines).
  // need to add type_fprint style functions in the future
  setbuf(stdout, NULL);
  if(argc > 1) {  //arguments on top of program name
    if(argc == 3) {
      yyin = fopen(argv[2], "r");
      if(!yyin) {
        fprintf(stderr, "cminor: error: no such file or directory: %s\n", argv[2]);
        fprintf(stderr, "cminor: error: no input files\n");
        exit(1);
      }
    }
    else if(argc > 4) {
      fprintf(stderr, "Extraneous arguments\n");
      fprintf(stderr, "See 'cminor --help'\n");
      exit(1);
    }

    if(strcmp(argv[1], "--help") == 0) {
      printHelp();
    }
    else if(strcmp(argv[1], "-scan") == 0) {
      if(yyin == stdin) printf("Enter in CMinor code to see if it scans.\n");

      while(1) {
        int token = yylex();
        if(!token) {
          break;
        }
        switch (token) {
          case TCHARLIT:
          printf("CHARACTER_LITERAL: %c\n", yylval.charLit);
          break;
          case TSTRLIT:
          printf("STRING_LITERAL: %s\n", yylval.strLit);
          free(yylval.strLit);
          break;
          case TIDENT:
          printf("IDENTIFIER: %s\n", yylval.strLit);
          free(yylval.strLit);
          break;
          case TINTLIT:
          printf("INTEGER_LITERAL: %ld\n", yylval.intLit);
          break;
          default:
          // a bit hacky: Bison tokens start at 258
          printf("%s\n", TOKEN_STRING[token - 258]);
          break;
        }
      }
    }
    else if(strcmp(argv[1], "-parse") == 0) {
      int outCode = yyparse();
      decl_print(programRoot, 0);
      decl_free(programRoot);
      exit(outCode);
    }
    else if(strcmp(argv[1], "-resolve") == 0) {
      if(yyparse()) {
        exit(1);
      }
      scope_enter();
      decl_resolve(programRoot, SYMBOL_GLOBAL, 0);
      scope_exit();
      decl_free(programRoot);
      if(resolve_error_count) {
        printf("Resolve error count: %d\n", resolve_error_count);
        exit(1);
      }
    }
    else if(strcmp(argv[1], "-typecheck") == 0) {
      if(yyparse()) {
        exit(1);
      }

      scope_enter();
      decl_resolve(programRoot, SYMBOL_GLOBAL, 0);
      scope_exit();
      if(resolve_error_count) {
        decl_free(programRoot);
        printf("Resolve error count: %d\n", resolve_error_count);
        exit(1);
      }
      decl_typecheck(programRoot);
      decl_free(programRoot);
      if(type_error_count) {
        printf("Type error count: %d\n", type_error_count);
        exit(1);
      }
    }
    else if(strcmp(argv[1], "-codegen") == 0) {
      if(argc != 4) {
        fprintf(stderr, "No file output name for option '%s'\n", argv[1]);
        exit(1);
      }

      FILE *f = fopen(argv[3], "w");
      if(!f) {
        fprintf(stderr, "Can't open output assembly file %s\n", argv[3]);
      }

      if(yyparse()) {
        exit(1);
      }
      scope_enter();
      decl_resolve(programRoot, SYMBOL_GLOBAL, 0);
      scope_exit();
      if(resolve_error_count) {
        decl_free(programRoot);
        printf("Resolve error count: %d\n", resolve_error_count);
        exit(1);
      }
      decl_typecheck(programRoot);
      if(type_error_count) {
        printf("Type error count: %d\n", type_error_count);
        exit(1);
      }
      assembly_codegen(programRoot, f);
      decl_free(programRoot);
      fclose(f);
    }
    else if(strcmp(argv[1], "-codegen_debug") == 0) {
      // enable codegen comments
      ASSEMBLY_COMMENT_FLAG = 1;
      if(argc != 4) {
        fprintf(stderr, "No file output name for option '%s'\n", argv[1]);
        exit(1);
      }

      FILE *f = fopen(argv[3], "w");
      if(!f) {
        fprintf(stderr, "Can't open output assembly file %s\n", argv[3]);
      }

      if(yyparse()) {
        exit(1);
      }
      scope_enter();
      decl_resolve(programRoot, SYMBOL_GLOBAL, 0);
      scope_exit();
      if(resolve_error_count) {
        decl_free(programRoot);
        printf("Resolve error count: %d\n", resolve_error_count);
        exit(1);
      }
      decl_typecheck(programRoot);
      if(type_error_count) {
        printf("Type error count: %d\n", type_error_count);
        exit(1);
      }
      assembly_codegen(programRoot, f);
      decl_free(programRoot);
      fclose(f);
    }
    // TODO: call system gcc to compile generated code into machine code
    else if(strcmp(argv[1], "-compile") == 0) {
      if(argc != 4) {
        fprintf(stderr, "No file output name for option '%s'\n", argv[1]);
        exit(1);
      }

      FILE *f = fopen(argv[3], "w");
      if(!f) {
        fprintf(stderr, "Can't open output assembly file %s\n", argv[3]);
      }

      if(yyparse()) {
        exit(1);
      }
      scope_enter();
      decl_resolve(programRoot, SYMBOL_GLOBAL, 0);
      scope_exit();
      if(resolve_error_count) {
        decl_free(programRoot);
        printf("Resolve error count: %d\n", resolve_error_count);
        exit(1);
      }
      decl_typecheck(programRoot);
      if(type_error_count) {
        printf("Type error count: %d\n", type_error_count);
        exit(1);
      }
      assembly_codegen(programRoot, f);
      decl_free(programRoot);
      fclose(f);
    }
    else {
      printGoHelp(argv[1]);
    }
  }
  else {
    printHelp();
  }
  fclose(yyin);
  return 0;
}
