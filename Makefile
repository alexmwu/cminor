# directories for grammar
GRAM_DIR = grammar
# scanner and parser pre (and post) generated file names
PARSER_PRE = parser
SCANNER_PRE = scanner

# token file
TOKEN_PATH = $(GRAM_DIR)/token.h

# scanner/parser inputs to Lex/Yacc and Lex/Yacc generated code
SCANNER_IN = $(GRAM_DIR)/$(SCANNER_PRE).l
PARSER_IN = $(GRAM_DIR)/$(PARSER_PRE).y
SCANNER_CODE = $(SCANNER_PRE).yy.c
PARSER_CODE = $(PARSER_PRE).tab.c

# all source and object files needed for compilation
GRAM_SRC = $(SCANNER_CODE) $(PARSER_CODE) main.c
GRAM_OBJ = $(GRAM_SRC:.c=.o)

# dependencies
DEPS = $(TOKEN_PATH) $(SCANNER_PRE).yy.h $(PARSER_PRE).tab.h
OUTPUT_BINARY = cminor

# tools used and their flags
CC = gcc
CCFLAGS = -Wall
GDBFLAG = -g
CFLAGS = -I./$(GRAM_DIR)
LEX = flex
YACC = bison
LEXFLAGS = -o $(SCANNER_PRE).yy.c
YACCFLAGS = -d -o $(PARSER_PRE).tab.c
# verbose and debug (better syntax errors)
YACCDEBUGFLAGS = -v -t


# make everything into a CMinor binary
all: $(GRAM_OBJ) $(DEPS)
	$(CC) $(CCFLAGS) $(GRAM_OBJ) -o $(OUTPUT_BINARY) $(CFLAGS)
# make everything into a CMinor binary that can be used with gdb
# need to rewrite commands because of dependencies on different
# versions of source files (e.g., bison output)
debug:
	$(LEX) $(LEXFLAGS) $(SCANNER_IN)
	$(YACC) $(YACCFLAGS) $(YACCDEBUGFLAGS) $(PARSER_IN)
	$(CC) $(CCFLAGS) -c $(GRAM_SRC) $(CFLAGS)
	$(CC) $(CCFLAGS) $(GRAM_OBJ) -o $(OUTPUT_BINARY) $(CFLAGS) $(GDBFLAG)

# make bison-generated parser with output
bison_debug: $(PARSER_IN) $(TOKEN_PATH)
	$(YACC) $(YACCFLAGS) $(YACCDEBUGFLAGS) $(PARSER_IN)


$(GRAM_OBJ): $(GRAM_SRC)
	$(CC) $(CCFLAGS) -c $(GRAM_SRC) $(CFLAGS)

$(PARSER_CODE) $(PARSER_PRE).tab.h: $(PARSER_IN) $(TOKEN_PATH)
	$(YACC) $(YACCFLAGS) $(PARSER_IN)
$(SCANNER_CODE) $(SCANNER_PRE).yy.h: $(SCANNER_IN) $(TOKEN_PATH) $(PARSER_PRE).tab.h
	$(LEX) $(LEXFLAGS) $(SCANNER_IN)
clean:
	rm $(SCANNER_PRE).yy.h $(PARSER_PRE).tab.h $(SCANNER_CODE) $(PARSER_CODE) $(PARSER_PRE).output $(GRAM_OBJ) $(OUTPUT_BINARY)
