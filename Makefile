# tools used and their flags
CC = gcc
GDBFLAG = -g
CFLAGS = -I./grammar
LEX = flex
YACC = bison
YACCFLAGS = -v

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
SCANNER_CODE = lex.yy.c
PARSER_CODE = $(PARSER_PRE).tab.c

# compiled scanner/parser objects
SCANNER_OBJ = $(GRAM_DIR)/$(SCANNER_PRE).yy.o
PARSER_OBJ = $(GRAM_DIR)/$(PARSER_PRE).tab.o

# all source and object files needed for compilation
GRAM_SRC = $(SCANNER_CODE) $(PARSER_CODE) main.c
GRAM_OBJ = $(GRAM_SRC:.c=.o)

# dependencies
DEPS = $(TOKEN_PATH) $(SCANNER_PRE).yy.h $(PARSER_PRE).tab.h
OUTPUT_BINARY = cminor



# make everything into a CMinor binary
all: $(GRAM_OBJ) $(DEPS)
	$(CC) $(GRAM_OBJ) -o $(OUTPUT_BINARY) $(CFLAGS)
# make everything into a CMinor binary that can be used with gdb
debug: $(GRAM_OBJ) $(DEPS)
	$(CC) $(GRAM_OBJ) -o $(OUTPUT_BINARY) $(CFLAGS) $(GDBFLAG)

# make bison-generated parser with output
bison_debug: $(PARSER_IN) $(TOKEN_PATH)
	$(YACC) $(YACCFLAGS) $(PARSER_IN)


$(GRAM_OBJ): $(GRAM_SRC)
	$(CC) -c $(GRAM_SRC) $(CFLAGS)

$(PARSER_CODE) $(PARSER_PRE).tab.h: $(PARSER_IN) $(TOKEN_PATH)
	$(YACC) $(PARSER_IN)
$(SCANNER_CODE) $(SCANNER_PRE).yy.h: $(SCANNER_IN) $(TOKEN_PATH)
	$(LEX) $(SCANNER_IN)
clean:
	rm $(SCANNER_OBJ) $(PARSER_OBJ) $(OUTPUT_BINARY)
