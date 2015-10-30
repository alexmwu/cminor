#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammar/token.h"
#include "scanner.yy.h"
#include "parser.tab.h"

extern FILE *yyin;
extern int yyparse();

// temporary help printout
void printHelp() {
  printf("The CMinor Compiler\n\n");
  printf("usage: cminor [--help|<command>] [filename]\n\n");
  printf("*Filenames are optional; the default is stdin\n");
  printf("These are optional commands for the CMinor compiler:\n");
  printf("\t-scan\tRun an optional file through CMinor scanner\n");
  printf("\t-parse\tRun an optional file through CMinor parser\n");
}

void printGoHelp(char *command) {
  printf("cminor: '%s' is not a cminor command. See 'cminor --help'.\n", command);
}

int main(int argc, char **argv) {
  yyin = stdin;
  // TODO: replace if-else statement handling with lib getopt (unistd.h)
  if(argc > 1) {  //arguments on top of program name
    if(argc == 3) {
      yyin = fopen(argv[2], "r");
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
          printf("CHARACTER_LITERAL: %c\n", handleText(yytext));
          break;
          case TSTRLIT:
          printf("STRING_LITERAL: %s\n", handleString(yytext, 0));
          break;
          case TIDENT:
          printf("IDENTIFIER: %s\n", handleString(yytext, 1));
          break;
          case TINTLIT:
          printf("INTEGER_LITERAL: %ld", handleInt(yytext));
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
      exit(outCode);
    }
    else {
      printGoHelp(argv[1]);
    }
  }
  else {
    printHelp();
  }

  return 0;
}
