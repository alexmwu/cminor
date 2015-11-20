#include <stdio.h>
#include "grammar/token.h"
#include "grammar/handle.h"
#include "scanner.yy.h"
#include "parser.tab.h"
#include "ast/decl.h"

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
  printf("\t-resolve\tRun an optional file through CMinor resolver and see if there are name resolution errors\n");
  printf("\t-typecheck\tRun an optional file through CMinor typechecker and see if there are type errors\n");
}

void printGoHelp(char *command) {
  printf("cminor: '%s' is not a cminor command. See 'cminor --help'.\n", command);
}

int main(int argc, char **argv) {
  yyin = stdin;
  if(argc > 1) {  //arguments on top of program name
    if(argc == 3) {
      yyin = fopen(argv[2], "r");
      if(!yyin) {
        fprintf(stderr, "cminor: error: no such file or directory: %s\n", argv[2]);
        fprintf(stderr, "cminor: error: no input files\n");
        exit(1);
      }
    }
    else if(argc > 3) {
      fprintf(stderr, "Extraneous arguments\n");
      printf("See 'cminor --help'\n");
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
      yyparse();
      scope_enter();
      decl_resolve(programRoot, SYMBOL_GLOBAL, 0);
      scope_exit();
      decl_free(programRoot);
      if(resolve_error_count) {
        printf("Resolve error count: %d\n", resolve_error_count);
        exit(1);
      }
      exit(0);
    }
    else if(strcmp(argv[1], "-typecheck") == 0) {
      yyparse();
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
      exit(0);
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
