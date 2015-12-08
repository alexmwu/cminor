#ifndef LIBRARY_H
#define LIBRARY_H

void print_integer(int x);
void print_string(const char *s);
void print_boolean(int b);
void print_character(char c);
void print_indent(int indent);
void fprint_integer(FILE *f, int x);
void fprint_string(FILE *f, const char *s);
void fprint_boolean(FILE *f, int b);
void fprint_character(FILE *f, char c);
void fprint_indent(FILE *f, int indent);
int integer_power(int x, int y);
int strcmp(const char* s1, const char* s2);

#endif
