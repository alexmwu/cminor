# directories for grammar
GRAM_DIR = grammar
# directories for ast
AST_DIR = ast
# scanner and parser pre (and post) generated file names
PARSER_PRE = parser
SCANNER_PRE = scanner

# token file
TOKEN_HEADER = $(GRAM_DIR)/token.h

# handling functions
HANDLE_HEADER = $(GRAM_DIR)/handle.h
HANDLE_SRC = $(GRAM_DIR)/handle.c
HANDLE_OBJ = handle.o

# AST library functions
AST_SRC = $(wildcard $(AST_DIR)/*.c)
AST_HEADER = $(wildcard $(AST_DIR)/*.h)
AST_OBJ = $(notdir $(AST_SRC:.c=.o))

# scanner/parser inputs to Lex/Yacc and Lex/Yacc generated code
SCANNER_IN = $(GRAM_DIR)/$(SCANNER_PRE).l
PARSER_IN = $(GRAM_DIR)/$(PARSER_PRE).y
SCANNER_SRC = $(SCANNER_PRE).yy.c
PARSER_SRC = $(PARSER_PRE).tab.c

# all source and object files needed for compilation
GRAM_SRC = $(SCANNER_SRC) $(PARSER_SRC) main.c
GRAM_OBJ = $(GRAM_SRC:.c=.o)

# dependencies
DEPS = $(TOKEN_HEADER) $(HANDLE_HEADER) $(SCANNER_PRE).yy.h $(PARSER_PRE).tab.h $(AST_HEADER)
OUTPUT_BINARY = cminor

# tools used and their flags
CC = gcc
CCDEBUGFLAGS = -Wall
GDBFLAG = -g -v
CFLAGS = -I./$(GRAM_DIR)
LEX = flex
YACC = bison
LEXFLAGS = -o $(SCANNER_PRE).yy.c
YACCFLAGS = -d -o $(PARSER_PRE).tab.c
# verbose and debug (better syntax errors)
YACCDEBUGFLAGS = -v -t


# make everything into a CMinor binary
all: $(DEPS) $(GRAM_OBJ) $(HANDLE_OBJ) $(AST_OBJ)
	$(CC) $(GRAM_OBJ) $(HANDLE_OBJ) $(AST_OBJ) -o $(OUTPUT_BINARY) $(CFLAGS)
# make everything into a CMinor binary that can be used with gdb
# need to rewrite commands because of dependencies on different
# versions of source files (e.g., bison output)
debug:
	$(LEX) $(LEXFLAGS) $(SCANNER_IN)
	$(YACC) $(YACCFLAGS) $(YACCDEBUGFLAGS) $(PARSER_IN)
	$(CC) $(CCDEBUGFLAGS) -c $(GRAM_SRC) $(HANDLE_SRC) $(AST_SRC) $(CFLAGS)
	$(CC) $(CCDEBUGFLAGS) $(GRAM_OBJ) $(HANDLE_OBJ) $(AST_OBJ) -o $(OUTPUT_BINARY) $(CFLAGS)

# make bison-generated parser with output
bison_debug: $(PARSER_IN) $(TOKEN_HEADER)
	$(YACC) $(YACCFLAGS) $(YACCDEBUGFLAGS) $(PARSER_IN)


$(GRAM_OBJ) $(HANDLE_OBJ) $(AST_OBJ): $(GRAM_SRC) $(HANDLE_SRC) $(AST_SRC)
	$(CC) -c $(GRAM_SRC) $(HANDLE_SRC) $(AST_SRC) $(CFLAGS)

$(PARSER_SRC) $(PARSER_PRE).tab.h: $(PARSER_IN) $(TOKEN_HEADER)
	$(YACC) $(YACCFLAGS) $(PARSER_IN)
$(SCANNER_SRC) $(SCANNER_PRE).yy.h: $(SCANNER_IN) $(TOKEN_HEADER) $(PARSER_PRE).tab.h
	$(LEX) $(LEXFLAGS) $(SCANNER_IN)
clean:
	rm $(SCANNER_PRE).yy.h $(PARSER_PRE).tab.h $(SCANNER_SRC) $(PARSER_SRC) $(PARSER_PRE).output $(GRAM_OBJ) $(AST_OBJ) $(OUTPUT_BINARY)
