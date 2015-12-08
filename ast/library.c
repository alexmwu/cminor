/* Copyright (C) 1991, 1996, 1997, 2003 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

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

void fprint_integer(FILE *f, int x) {
	fprintf(f, "%d",x);
}

/*TODO: write a function to handle \n and \0 so they get printed out like code (called print_code_string, for example*/
void fprint_string(FILE *f, const char *s) {
	fprintf(f, "%s",s);
}

void fprint_boolean(FILE *f, int b) {
	fprintf(f, "%s",b?"true":"false");
}

void fprint_character(FILE *f, char c) {
	fprintf(f, "%c",c);
}
void fprint_indent(FILE *f, int indent) {
  int i;
  for(i = 0; i < indent; i++) {
    fprintf(f, "\t");
  }
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

/*implementation found here: http://stackoverflow.com/questions/20004458/optimized-strcmp-implementation*/
int strcmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1==*s2))
        s1++,s2++;
    return *(const unsigned char*)s1-*(const unsigned char*)s2;
}

