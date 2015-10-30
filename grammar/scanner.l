%{
#include "../parser.tab.h"
#define TOKEN(t) (yylval.token = t)

char handleChar(char *c);
char *handleString(char *s, int type);
long handleInt(char *i);
%}

%option noyywrap
%option header-file="scanner.yy.h"

%x C_COMMENT

WHITESPACE [ \t\n\r]+
ALPHA [a-zA-Z]
ALPHA_UNDERSCORE [a-zA-Z_]
DIGIT [0-9]
CPPCOMMENT \/\/.*\n
IDENTIFIER {ALPHA_UNDERSCORE}({ALPHA_UNDERSCORE}|{DIGIT})*
CHAR ('.'|'\\.')
STRING \"(\\.|[^\\"])*\"
INTEGER {DIGIT}+

%%

array { return TOKEN(TARR); }
boolean { return TOKEN(TBOOL); }
char { return TOKEN(TCHAR); }
else { return TOKEN(TELSE); }
false { return TOKEN(TFALSE); }
for { return TOKEN(TFOR); }
function { return TOKEN(TFUNC); }
if { return TOKEN(TIF); }
integer { return TOKEN(TINT); }
print { return TOKEN(TPRINT); }
return { return TOKEN(TRET); }
string { return TOKEN(TSTR); }
true { return TOKEN(TTRUE); }
void { return TOKEN(TVOID); }
while { return TOKEN(TWHILE); }
; { return TOKEN(TSEMI); }
, { return TOKEN(TCOMMA); }
{WHITESPACE} { }
{IDENTIFIER} { yylval.ident = handleString(yytext, 1); return TOKEN(TIDENT); }
{CPPCOMMENT} { }
"/*" { BEGIN(C_COMMENT); }
<C_COMMENT>"*/" { BEGIN(INITIAL); }
<C_COMMENT>. { }
<C_COMMENT><<EOF>> { fprintf(stderr, "SCAN_ERROR: Trailing C-Comment\n"); exit(1); }
{CHAR} { yylval.charLit = handleChar(yytext); return TOKEN(TCHARLIT); }
{STRING} { yylval.strLit = handleString(yytext, 0); return TOKEN(TSTRLIT); }
{INTEGER} { yylval.intLit = handleInt(yytext); return TOKEN(TINTLIT); }
: { return TOKEN(TCOL); }
= { return TOKEN(TEQ); }
\{ { return TOKEN(TLBRACE); }
\} { return TOKEN(TRBRACE); }
\( { return TOKEN(TLPAREN); }
\) { return TOKEN(TRPAREN); }
\[ { return TOKEN(TLBRACK); }
\] { return TOKEN(TRBRACK); }
\+\+ { return TOKEN(TPLUSPLUS); }
-- { return TOKEN(TMINMIN); }
\^ { return TOKEN(TEXP); }
\+ { return TOKEN(TPLUS); }
- { return TOKEN(TMIN); }
! { return TOKEN(TNOT); }
\* { return TOKEN(TMUL); }
\/ { return TOKEN(TDIV); }
% { return TOKEN(TMOD); }
\< { return TOKEN(TLT); }
\<= { return TOKEN(TLE); }
> { return TOKEN(TGT); }
>= { return TOKEN(TGE); }
== { return TOKEN(TEQ); }
!= { return TOKEN(TNE); }
&& { return TOKEN(TAND); }
\|\| { return TOKEN(TOR); }
. { fprintf(stderr, "SCAN_ERROR: %s is not a valid token\n", yytext); exit(1); }

%%

// scanner tools

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

long handleInt(char *i) {
  char *end;
  // 0 means allow C integer constant formats
  long ret = strtol(i, &end, 0);
  if(!ret && *end == i) {
    fprintf(stderr, "Integer scan error");
    exit(1);
  }

  return ret;
}
