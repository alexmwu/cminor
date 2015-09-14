%{
#include <stdio.h>
#include "token.h"
%}

WHITESPACE [ \t]+
COMMENT (\/\*.*\*\/)|(\/\/.*\n)
ALPHA [a-zA-Z]
ALPHA_UNDERSCORE [a-zA-Z_]
DIGIT [0-9]
DIGIT_NOZERO [1-9]
IDENTIFIER {ALPHA_UNDERSCORE}({ALPHA_UNDERSCORE}|{DIGIT})*
CHAR ('.'|'\\n'|'\\0')
STRING \".*\"
INTEGER 0|-?{DIGIT_NOZERO}{DIGIT}*

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
{WHITESPACE} { return WHITESPACE; }
{COMMENT} { return COMMENT; }
{IDENTIFIER} { return IDENTIFIER; }
{CHAR} { return CHAR_LITERAL; }
{STRING} { return STRING_LITERAL; }
{INTEGER} { return INTEGER_LITERAL; }
: { return DECLARATION; }
= { return ASSIGNMENT; }
\( { return LEFT_PAREN; }
\) { return RIGHT_PAREN; }
\+\+ { return PLUSPLUS; }
-- { return MINUSMINUS; }
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
. { return PARSE_ERROR; }
