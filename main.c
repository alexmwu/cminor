#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include "grammar/token.h"*/
#include "scanner.yy.h"
#include "parser.tab.h"

extern FILE *yyin;

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
    return (char) -1;
  }
}

// need to free string
char *handleString(char *s) {
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
      if(*ptr == 'n') {
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
    return NULL;  // return NULL if final string length is greater than 255
  }
  return retStr;
}

int main(int argc, char **argv) {
  yyin = stdin;
  if(argc > 1) {  //arguments on top of program name
    if(strcmp(argv[1], "-scan") == 0 && argc > 2) {
      yyin = fopen(argv[2], "r");
    }
  }

  if(yyin == stdin) printf("Enter in CMinor code to see if it scans.\n");

  while(1) {
    int token = yylex();
    if(!token) {
      break;
    }
    switch (token) {
      char c;
      char *s;
      case CHARLIT:
        c = handleChar(yytext);
        if(c == (char) -1) {
          fprintf(stderr, "Unknown/malformed CHARLIT scanned in: %c\n", c);
          exit(1);
        }
        printf("CHARLIT: %c\n", c);
        break;
      case STRLIT:
        s = handleString(yytext);
        if(s == NULL) {
          fprintf(stderr, "SCAN_ERROR: STRLIT has exceeded the max size of 255\n");
          exit(1);
        }
        else {
          printf("STRLIT: %s\n", s);
        }
        free(s);  // TODO: free for now - may need to do other things with it later
        break;
      case IDENT:
        s = handleString(yytext);
        if(s == NULL) {
          fprintf(stderr, "SCAN_ERROR: IDENT has exceeded the max size of 255\n");
          exit(1);
        }
        else {
          printf("IDENT\n");
        }
        free(s);  // TODO: free for now - may need to do other things with it later
        break;
      default:
        printf("%d\n", token);
        break;
    }
  }
  return 0;
}
