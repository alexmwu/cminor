%{
#include "../parser.tab.h"
#include "handle.h"
#define TOKEN(t) (yylval.token = t)
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
== { return TOKEN(TEQEQ); }
!= { return TOKEN(TNE); }
&& { return TOKEN(TAND); }
\|\| { return TOKEN(TOR); }
. { fprintf(stderr, "SCAN_ERROR: %s is not a valid token\n", yytext); exit(1); }

%%
