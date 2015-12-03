/*
This is the standard library of functions for cminor, implemented in C.
The print statement in C-minor expects there to exist a function
for each type that can be printed.  So, the following cminor code:

x: int = 10;
b: boolean = true;
s: string = "hello";

print x, b, s;

Is effectively translated to the following C code:

print_integer(x);
print_boolean(b);
print_string(s);

And the following cminor code:

x = a ^ b;

Is effectively this C code:

x = integer_power(a,b);
*/

#include <stdio.h>

void print_indent(int indent) {
  int i;
  for(i = 0; i < indent; i++) {
    printf("\t");
  }
}

void fprint_indent(FILE *f, int indent) {
  int i;
  for(i = 0; i < indent; i++) {
    fprintf(f, "\t");
  }
}

void print_integer( int x )
{
	printf("%d",x);
}

/*TODO: write a function to handle \n and \0 so they get printed out like code (called print_code_string, for example*/
void print_string( const char *s )
{
	printf("%s",s);
}

void print_boolean( int b )
{
	printf("%s",b?"true":"false");
}

void print_character( char c )
{
	printf("%c",c);
}

int integer_power( int x, int y )
{
	int result = 1;
	while(y>0) {
		result = result * x;
		y = y -1;
	}
	return result;
}

