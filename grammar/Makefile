LEX = flex
YACC = bison

CC = gcc
CFLAGS = -I.
GDBFLAG = -g

all: lex.yy.c token.h
	$(CC) lex.yy.c parser.tab.c -o cminor $(CFLAGS)
debug: lex.yy.c token.h
	$(CC) lex.yy.c parser.tab.c -o cminor $(CFLAGS) $(GDBFLAG)
parser.tab.c: parser.y token.h
	$(YACC) parser.y
lex.yy.c: scanner.l token.h
	$(LEX) scanner.l
clean:
	rm lex.yy.c parser.tab.c cminor
