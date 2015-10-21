%{
/*C preamble*/
#include "scanner.yy.h"
extern int yyerror(char *);
%}

%token INTEGER STRING CHAR BOOLEAN ARRAY VOID
%token TRUE FALSE IF ELSE WHILE FOR
%token FUNCTION RETURN PRINT

%token SEMICOLON COMMA COLON EQUALS
%token LEFT_BRACE RIGHT_BRACE LEFT_PAREN RIGHT_PAREN LEFT_BRACKET RIGHT_BRACKET

%token INTEGER_LITERAL STRING_LITERAL CHAR_LITERAL
%token IDENTIFIER

%token PLUSPLUS MINUSMINUS EXPONENTIATION PLUS MINUS MULTIPLICATION DIVISION MODULUS
%token LT LE GT GE EQ NE AND OR NOT

%nonassoc IFX
%nonassoc ELSE

%start program

%%

program: decl_list
       ;

decl_list: decl_list decl
         | /*nothing*/
         ;

decl: IDENTIFIER COLON type EQUALS expr SEMICOLON
    | IDENTIFIER COLON type SEMICOLON
    | IDENTIFIER COLON type EQUALS LEFT_BRACE stmt_list RIGHT_BRACE
    ;

type: STRING
    | INTEGER
    | CHAR
    | BOOLEAN
    | ARRAY LEFT_BRACE INTEGER_LITERAL RIGHT_BRACE type /*TOOD: see if only fixed sized numbers (integer_literal). i.e., no expressions?*/
    | FUNCTION type LEFT_PAREN optional_param_list RIGHT_PAREN
    | VOID
    ;

optional_param_list: /*nothing*/
          | param_list
          ;

param_list: decl
          | param_list COMMA param
          ;

param: IDENTIFIER COLON type
     ;

stmt: decl
    | expr SEMICOLON
    | FOR LEFT_PAREN optional_expr SEMICOLON optional_expr SEMICOLON optional_expr RIGHT_PAREN stmt
    | LEFT_BRACE stmt_list RIGHT_BRACE
    /* TODO: fix the below dangling else */
    | if_stmt
    | RETURN expr SEMICOLON
    | PRINT expr_list SEMICOLON
    ;

if_stmt: IF LEFT_PAREN expr RIGHT_PAREN stmt %prec IFX
       | IF LEFT_PAREN expr RIGHT_PAREN stmt ELSE stmt
       ;

stmt_list: stmt
         | stmt_list stmt
         ;

expr: IDENTIFIER
    ;

optional_expr: /*nothing*/
             | expr
             ;

expr_list: expr
         | expr_list COMMA expr
         ;
