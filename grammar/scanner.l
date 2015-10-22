%{
#include "../parser.tab.h"
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

array { return TARR; }
boolean { return TBOOL; }
char { return TCHAR; }
else { return TELSE; }
false { return TFALSE; }
for { return TFOR; }
function { return TFUNC; }
if { return TIF; }
integer { return TINT; }
print { return TPRINT; }
return { return TRET; }
string { return TSTR; }
true { return TTRUE; }
void { return TVOID; }
while { return TWHILE; }
; { return TSEMI; }
, { return TCOMMA; }
{WHITESPACE} { }
{IDENTIFIER} { return TIDENT; }
{CPPCOMMENT} { }
"/*" { BEGIN(C_COMMENT); }
<C_COMMENT>"*/" { BEGIN(INITIAL); }
<C_COMMENT>. { }
<C_COMMENT><<EOF>> { fprintf(stderr, "SCAN_ERROR: Trailing C-Comment\n"); exit(1); }
{CHAR} { return TCHARLIT; }
{STRING} { return TSTRLIT; }
{INTEGER} { return TINTLIT; }
: { return TCOL; }
= { return TEQ; }
\{ { return TLBRACE; }
\} { return TRBRACE; }
\( { return TLPAREN; }
\) { return TRPAREN; }
\[ { return TLBRACK; }
\] { return TRBRACK; }
\+\+ { return TPLUSPLUS; }
-- { return TMINMIN; }
\^ { return TEXP; }
\+ { return TPLUS; }
- { return TMIN; }
! { return TNOT; }
\* { return TMULT; }
\/ { return TDIV; }
% { return TMOD; }
\< { return TLT; }
\<= { return TLE; }
> { return TGT; }
>= { return TGE; }
== { return TEQ; }
!= { return TNE; }
&& { return TAND; }
\|\| { return TOR; }
. { fprintf(stderr, "SCAN_ERROR: %s is not a valid token\n", yytext); exit(1); }

%%
