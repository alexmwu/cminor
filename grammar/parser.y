%{
/*C preamble*/
#include "scanner.yy.h"
extern int yyerror(char *);
%}

%token INT STR CHAR BOOL ARR VOID
%token TRUE FALSE IF ELSE WHILE FOR
%token FUNC RET PRINT

%token SEMI COMMA COL
%token LBRACE RBRACE LPAREN RPAREN LBRACK RBRACK

%token INTLIT STRLIT CHARLIT
%token IDENT

%token PLUSPLUS MINMIN EXP PLUS MIN MULT DIV MOD
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

decl: IDENT COL type EQ expr SEMI
    | IDENT COL type SEMI
    | IDENT COL type EQ LBRACE stmt_list RBRACE
    ;

type: STR
    | INT
    | CHAR
    | BOOL
    | ARR LBRACE INTLIT RBRACE type /*TOOD: see if only fixed sized numbers (integer_literal). i.e., no expressions?*/
    | FUNC type LPAREN optional_param_list RPAREN
    | VOID
    ;

optional_param_list: /*nothing*/
          | param_list
          ;

param_list: decl
          | param_list COMMA param
          ;

param: IDENT COL type
     ;

stmt: decl
    | expr SEMI
    | FOR LPAREN optional_expr SEMI optional_expr SEMI optional_expr RPAREN stmt
    | LBRACE stmt_list RBRACE
    /* TODO: fix the below dangling else */
    | if_stmt
    | RET expr SEMI
    | PRINT expr_list SEMI
    ;

if_stmt: IF LPAREN expr RPAREN stmt %prec IFX
       | IF LPAREN expr RPAREN stmt ELSE stmt
       ;

stmt_list: stmt
         | stmt_list stmt
         ;

expr: IDENT
    ;

optional_expr: /*nothing*/
             | expr
             ;

expr_list: expr
         | expr_list COMMA expr
         ;
