%{
#include <stdio.h>
%}

WHITESPACE [ \t]+
COMMENTS (/\*.**/ | //.*\n)
ALPHA [a-zA-Z]
ALPHA_UNDERSCORE [a-zA-Z_]
DIGIT [0-9]
IDENTIFIER {ALPHA_UNDERSCORE}({ALPHA_UNDERSCORE} | {DIGIT})


%%

array {}
boolean {}
char {}
else {}
false {}
for {}
function {}
if {}
integer {}
print {}
return {}
string {}
true {}
void {}
while {}

