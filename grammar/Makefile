CC = gcc
LEX = flex
CFLAGS = -I.
GDBFLAG = -g

all: lex.yy.c token.h
	$(CC) lex.yy.c -o cminor $(CFLAGS)
debug: lex.yy.c token.h
	$(CC) lex.yy.c -o cminor $(CFLAGS) $(GDBFLAG)
lex.yy.c: scanner.l token.h
	$(LEX) scanner.l
clean:
	rm lex.yy.c cminor
