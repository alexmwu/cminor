%{
/*C preamble*/
#include "scanner.yy.h"
//TODO: better error messages (e.g., line number and line of error)
void yyerror(const char *s) { printf("ERROR: %s\n", s); }
%}

%token TINT TSTR TCHAR TBOOL TARR TVOID
%token TTRUE TFALSE TIF TELSE TWHILE TFOR
%token TFUNC TRET TPRINT

%token TSEMI TCOMMA TCOL TEQ
%token TLBRACE TRBRACE TLPAREN TRPAREN TLBRACK TRBRACK

%token TINTLIT TSTRLIT TCHARLIT
%token TIDENT

%token TPLUSPLUS TMINMIN TEXP TPLUS TMIN TMULT TDIV TMOD
%token TLT TLE TGT TGE TEQEQ TNE TAND TOR TNOT

%nonassoc TIFX
%nonassoc TELSE

%start program

%%

program: decl_list
       ;

decl_list: decl_list decl
         | /*nothing*/
         ;

decl: TIDENT TCOL type TEQ expr TSEMI
    | TIDENT TCOL type TSEMI
    | TIDENT TCOL type TEQ TLBRACE stmt_list TRBRACE
    ;

type: TSTR
    | TINT
    | TCHAR
    | TBOOL
    | TARR TLBRACE TINTLIT TRBRACE type /*TOOD: see if only fixed sized numbers (integer_literal). i.e., no expressions?*/
    | TFUNC type TLPAREN optional_param_list TRPAREN
    | TVOID
    ;

optional_param_list: /*nothing*/
          | param_list
          ;

param_list: decl
          | param_list TCOMMA param
          ;

param: TIDENT TCOL type
     ;

stmt: decl
    | expr TSEMI
    | TFOR TLPAREN optional_expr TSEMI optional_expr TSEMI optional_expr TRPAREN stmt
    | TLBRACE stmt_list TRBRACE
    /* TODO: fix the below dangling else */
    | if_stmt
    | TRET expr TSEMI
    | TPRINT expr_list TSEMI
    ;

if_stmt: TIF TLPAREN expr TRPAREN stmt %prec TIFX
       | TIF TLPAREN expr TRPAREN stmt TELSE stmt
       ;

stmt_list: stmt
         | stmt_list stmt
         ;

expr: TIDENT
    ;

optional_expr: /*nothing*/
             | expr
             ;

expr_list: expr
         | expr_list TCOMMA expr
         ;
