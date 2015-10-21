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
STRING \".*\"
INTEGER {DIGIT}+

%%

array { return ARR; }
boolean { return BOOL; }
char { return CHAR; }
else { return EL; }
false { return FALSE; }
for { return FOR; }
function { return FUNC; }
if { return IF; }
integer { return INT; }
print { return PRINT; }
return { return RET; }
string { return STR; }
true { return TRUE; }
void { return VOID; }
while { return WHILE; }
; { return SEMI; }
, { return COMMA; }
{WHITESPACE} { }
{IDENTIFIER} { return IDENT; }
{CPPCOMMENT} { }
"/*" { BEGIN(C_COMMENT); }
<C_COMMENT>"*/" { BEGIN(INITIAL); return CCOMMENT; }
<C_COMMENT>. { }
{CHAR} { return CHARLIT; }
{STRING} { return STRLIT; }
{INTEGER} { return INTLIT; }
: { return COL; }
= { return EQ; }
\{ { return LBRACE; }
\} { return RBRACE; }
\( { return LPAREN; }
\) { return RPAREN; }
\[ { return LBRACK; }
\] { return RBRACK; }
\+\+ { return PLUSPLUS; }
-- { return MINMIN; }
\^ { return EXP; }
\+ { return PLUS; }
- { return MIN; }
! { return NOT; }
\* { return MULT; }
\/ { return DIV; }
% { return MOD; }
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
