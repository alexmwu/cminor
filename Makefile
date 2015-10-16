LEX = flex
YACC = bison
GRAM_DIR = grammar
PARSER_PRE = parser
SCANNER_PRE = scanner

TOKEN_PATH = $(GRAM_DIR)/token.h

SCANNER_IN = $(GRAM_DIR)/$(SCANNER_PRE).l
PARSER_IN = $(GRAM_DIR)/$(PARSER_PRE).y
SCANNER_CODE = lex.yy.c
PARSER_CODE = $(PARSER_PRE).tab.c

SCANNER_OBJ = $(GRAM_DIR)/$(SCANNER_PRE).yy.o
PARSER_OBJ = $(GRAM_DIR)/$(PARSER_PRE).tab.o

GRAM_SRC = $(SCANNER_CODE) $(PARSER_CODE) main.c
GRAM_OBJ = $(GRAM_SRC:.c=.o)

DEPS = $(TOKEN_PATH) $(PARSER_PRE).tab.h
OUTPUT_BINARY = cminor

CC = gcc
GDBFLAG = -g
CFLAGS = -I./grammar

all: $(GRAM_OBJ) $(DEPS)
	$(CC) $(GRAM_OBJ) -o $(OUTPUT_BINARY) $(CFLAGS)
debug: $(GRAM_OBJ) $(DEPS)
	$(CC) $(GRAM_OBJ) -o $(OUTPUT_BINARY) $(CFLAGS) $(GDBFLAG)

$(GRAM_OBJ): $(GRAM_SRC)
	$(CC) -c $(GRAM_SRC) $(CFLAGS)

$(PARSER_CODE) $(PARSER_PRE).tab.h: $(PARSER_IN) $(TOKEN_PATH)
	$(YACC) $(PARSER_IN)
$(SCANNER_CODE): $(SCANNER_IN) $(TOKEN_PATH)
	$(LEX) $(SCANNER_IN)
clean:
	rm $(SCANNER_OBJ) $(PARSER_OBJ) $(OUTPUT_BINARY)
