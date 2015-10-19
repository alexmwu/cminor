%{
/* C preamble */
#include "grammar/token.h"
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

%%

/* Rule 1*/
program: decl_list
       ;

/* Rule 2*/
decl_list: decl_list decl
         | /* nothing */
         ;

/* Rule 3*/
decl: IDENTIFIER COLON type EQUALS expr SEMICOLON
    | IDENTIFIER COLON type SEMICOLON
    | IDENTIFIER COLON type EQUALS LEFT_BRACE stmt_list RIGHT_BRACE
    ;

/* Rule 4*/
type: STRING
    | INTEGER
    | CHAR
    | BOOLEAN
    | ARRAY LEFT_BRACE INTEGER_LITERAL RIGHT_BRACE type /* TOOD: see if only fixed sized numbers (integer_literal). i.e., no expressions? */
    | FUNCTION type LEFT_PAREN optional_param_list RIGHT_PAREN
    | VOID
    ;

/* Rule 5*/
optional_param_list: /* nothing */
          | param_list
          ;

/* Rule 6*/
param_list: decl
          | param_list COMMA param
          ;

/* Rule 7*/
param: IDENTIFIER COLON type
     ;

/* Rule 8*/
stmt: decl
    | expr SEMICOLON
    | FOR LEFT_PAREN optional_expr SEMICOLON optional_expr SEMICOLON optional_expr RIGHT_PAREN stmt
    | LEFT_BRACE stmt_list RIGHT_BRACE
    /* TODO: fix the below dangling else */
    | IF LEFT_PAREN expr RIGHT_PAREN stmt ELSE stmt
    | IF LEFT_PAREN expr RIGHT_PAREN stmt
    | RETURN expr SEMICOLON
    | PRINT expr_list SEMICOLON
    ;

/* Rule 9*/
stmt_list: stmt
         | stmt_list stmt
         ;

/* Rule 10*/
expr: expr COMMA
    | IDENTIFIER
    ;

/* Rule 11*/
optional_expr: /* nothing */
             | expr
             ;

/* Rule 12*/
expr_list: expr
         | expr_list expr
         ;
