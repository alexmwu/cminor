%{
#include <stdio.h>
#include "token.h"
%}

%option noyywrap

%x C_COMMENT

WHITESPACE [ \t\n\r]+
ALPHA [a-zA-Z]
ALPHA_UNDERSCORE [a-zA-Z_]
DIGIT [0-9]
CPPCOMMENT \/\/.*\n
IDENTIFIER {ALPHA_UNDERSCORE}({ALPHA_UNDERSCORE}|{DIGIT})*
CHAR ('.'|'\\.')
STRING \".*\"
INTEGER {DIGIT}+

%%

array { return ARRAY; }
boolean { return BOOLEAN; }
char { return CHAR; }
else { return ELSE; }
false { return FALSE; }
for { return FOR; }
function { return FUNCTION; }
if { return IF; }
integer { return INTEGER; }
print { return PRINT; }
return { return RETURN; }
string { return STRING; }
true { return TRUE; }
void { return VOID; }
while { return WHILE; }
; { return SEMICOLON; }
, { return COMMA; }
{WHITESPACE} { return WHITESPACE; }
{IDENTIFIER} { return IDENTIFIER; }
{CPPCOMMENT} { return CPPCOMMENT; }
"/*" { BEGIN(C_COMMENT); }
<C_COMMENT>"*/" { BEGIN(INITIAL); return CCOMMENT; }
<C_COMMENT>. { }
{CHAR} { return CHAR_LITERAL; }
{STRING} { return STRING_LITERAL; }
{INTEGER} { return INTEGER_LITERAL; }
: { return DECLARATION; }
= { return ASSIGNMENT; }
\{ { return LEFT_BRACE; }
\} { return RIGHT_BRACE; }
\( { return LEFT_PAREN; }
\) { return RIGHT_PAREN; }
\[ { return LEFT_BRACKET; }
\] { return RIGHT_BRACKET; }
\+\+ { return PLUSPLUS; }
-- { return MINUSMINUS; }
\^ { return EXPONENTIATION; }
\+ { return PLUS; }
- { return MINUS; }
\* { return MULTIPLICATION; }
\/ { return DIVISION; }
% { return MODULUS; }
\< { return LT; }
\<= { return LE; }
> { return GT; }
>= { return GE; }
== { return EQ; }
!= { return NE; }
&& { return AND; }
\|\| { return OR; }
. { return SCAN_ERROR; }

%%

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
    TOKEN token = yylex();
    if(!token) {
      break;
    }
    switch (token) {
      char c;
      char *s;
      case CHAR_LITERAL:
        c = handleChar(yytext);
        if(c == (char) -1) {
          fprintf(stderr, "Unknown/malformed CHARACTER_LITERAL scanned in: %c\n", c);
          exit(1);
        }
        printf("CHAR_LITERAL: %c\n", c);
        break;
      case STRING_LITERAL:
        s = handleString(yytext);
        if(s == NULL) {
          fprintf(stderr, "SCAN_ERROR: STRING_LITERAL has exceeded the max size of 255\n");
          exit(1);
        }
        else {
          printf("STRING_LITERAL: %s\n", s);
        }
        free(s);  // TODO: free for now - may need to do other things with it later
        break;
      case IDENTIFIER:
        s = handleString(yytext);
        if(s == NULL) {
          fprintf(stderr, "SCAN_ERROR: IDENTIFIER has exceeded the max size of 255\n");
          exit(1);
        }
        else {
          printf("IDENTIFIER\n");
        }
        free(s);  // TODO: free for now - may need to do other things with it later
        break;
      case WHITESPACE:
        break;
      case SCAN_ERROR:
        fprintf(stderr, "%s: %s is not a valid character.\n", TOKEN_STRING[token], yytext);
        exit(1);
        break;
      default:
        printf("%s\n", TOKEN_STRING[token]);
        break;
    }
  }
  return 0;
}
