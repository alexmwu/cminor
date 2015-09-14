CC = gcc
LEX = flex
CFLAGS = -I.
all: lex.yy.c token.h
	$(CC) lex.yy.c -o cminor $(CFLAGS)
lex.yy.c: scanner.l token.h
	$(LEX) scanner.l
clean:
	rm lex.yy.c cminor
