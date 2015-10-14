%{
/* C preamble */
#include "token.h"
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

program: decl_list
       ;

decl_list: decl_list decl
         | /* nothing */
         ;

decl: IDENTIFIER COLON type EQUALS expr SEMICOLON
    | IDENTIFIER COLON type SEMICOLON
    | IDENTIFIER COLON type EQUALS LEFT_BRACE stmt_list RIGHT_BRACE
    ;

/* declarations just for params (i.e., no definitions) */
param_decl: IDENTIFIER COLON type


type: STRING
    | INTEGER
    | CHAR
    | BOOLEAN
    | ARRAY /* TODO: need to see what Array type looks like */
    | FUNCTION type LEFT_PAREN optional_param_list RIGHT_PAREN
    | VOID
    ;

optional_param_list: /* nothing */
          | param_list
          ;

param_list: decl
          | param_list COMMA decl
          ;

param: IDENTIFIER COLON type
     ;

stmt: decl
    | expr SEMICOLON
    | FOR LEFT_PAREN optional_expr SEMICOLON optional_expr SEMICOLON optional_expr RIGHT_PAREN stmt
    | LEFT_BRACE stmt_list RIGHT_BRACE
    /* TODO: fix the below dangling else */
    | IF LEFT_PAREN expr RIGHT_PAREN stmt ELSE stmt
    | IF LEFT_PAREN expr RIGHT_PAREN stmt
    ;

expr:
/* TODO: expr */
optional_expr: /* nothing */
             | expr
             ;
