/*
 *Test that the scanner works on all valid CMinor programs
 */
#include "token.h"
#include <stdio.h>
#include "lex.yy.c"
int main(int argc, char **argv) {
  while(1) {
    TOKEN token = yylex();
    if(!token)
      break;
    else
      printf("%s\n", str(token));
  }
}
