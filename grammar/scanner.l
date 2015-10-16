%{
#include "grammar/token.h"
%}

%option noyywrap
%option header-file="lex.yy.h"

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
: { return COLON; }
= { return EQUALS; }
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
! { return NOT; }
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
