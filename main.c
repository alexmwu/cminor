#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammar/token.h"
#include "scanner.yy.h"
#include "parser.tab.h"

extern FILE *yyin;
extern int yyparse();

char handleChar(char *c) {
  // length should only be 3 or 4
  int len = strlen(c);
  if(len == 3) {
    // if character is just a single backslash
    if(c[1] == '\\') {
      return (char) -1;
    }
    return c[1];  // c should look like ' + char + '
  }
  else if(len == 4) {   // c should look like ' + \ + char + '
    if(c[2] == '0') {
      return (char) 0;  // ASCII code for NULL
    }
    else if(c[2] == 'n') {
      return (char) 10;  // ASCII code for NULL
    }
    else {
      return c[2];
    }
  }
  else {
    fprintf(stderr, "Unknown/malformed CHARLIT scanned in: %s\n", c);
    exit(1);
  }
}

// need to free string
// type 0 is string literal; 1 is identifier
char *handleString(char *s, int type) {
  int len = strlen(s);
  char *retStr = malloc(len * sizeof(char));
  char *ptr = s;
  int i = 0;
  while(*ptr) {
    if(*ptr == '"') {
      ptr++;
      continue;
    }
    else if(*ptr == '\\') {
      ptr++;  // move one ahead for actual value
      if(!ptr) {
        fprintf(stderr, "SCAN_ERROR: STRING_LITERAL terminated on '\\'\n");
      }
      else if(*ptr == 'n') {
        retStr[i++] = '\n';
      }
      else if(*ptr == '0') {
        retStr[i++] = '\0';
      }
      else {
        retStr[i++] = *ptr;
      }
      ptr++;
    }
    else {
      retStr[i++] = *ptr;
      ptr++;
    }
  }
  retStr[i] = '\0';
  if(strlen(retStr) > 255) {
    free(retStr);
    if(type == 0)
      fprintf(stderr, "SCAN_ERROR: STRING_LITERAL has exceeded the max size of 255\n");
    else if(type == 1)
      fprintf(stderr, "SCAN_ERROR: IDENTIFIER has exceeded the max size of 255\n");
    else
      fprintf(stderr, "Bad token type (should be string or ident)\n");
    exit(1);
  }
  return retStr;
}

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
  // TODO: replace if-else statement handling with lib getopt
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
          char c;
          char *s;
          case TCHARLIT:
          c = handleChar(yytext);
          printf("CHARACTER_LITERAL: %c\n", c);
          break;
          case TSTRLIT:
          s = handleString(yytext, 0);
          printf("STRING_LITERAL: %s\n", s);
          free(s);  // TODO: free for now - may need to do other things with it later
          break;
          case TIDENT:
          s = handleString(yytext, 1);
          printf("IDENTIFIER: %s\n", s);
          free(s);  // TODO: free for now - may need to do other things with it later
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
