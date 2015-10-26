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

%token TPLUSPLUS TMINMIN TEXP TPLUS TMIN TMUL TDIV TMOD
%token TLT TLE TGT TGE TEQEQ TNE TAND TOR TNOT

/*
 *Operator precedence (should achieve more C-like precedence at some point;
 *spec will change as needed)
 *As detailed here: http://ee.hawaii.edu/~tep/EE160/Book/chap5/subsection2.1.4.1.html
 */

%nonassoc TNOELSE
%right TEQ
%left TOR
%left TAND
/*The C-spec separates equality checks (==, !=) and comparisons (<, <=, etc.)*/
%left TLT TLE TGT TGE TEQEQ TNE
%left TPLUS TMIN
%left TMUL TDIV TMOD
%right TEXP
/*
 *C does not separate postfix increment or decrement from unary minus or logival not
 *(i.e., they have the same precedence)
 */
%right TUMIN TNOT
%right TPLUSPLUS TMINMIN

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
    | TARR TLBRACE TINTLIT TRBRACE type /*TODO: see if only fixed sized numbers (integer_literal). i.e., no expressions?*/
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
    | TLBRACE TRBRACE
    | if_stmt
    | TRET expr TSEMI
    | TPRINT expr_list TSEMI
    ;

if_stmt: TIF TLPAREN expr TRPAREN stmt %prec TNOELSE
       | TIF TLPAREN expr TRPAREN stmt TELSE stmt
       ;

stmt_list: stmt
         | stmt_list stmt
         ;

expr: TIDENT
    /*atomic types*/
    | TTRUE
    | TFALSE
    | TINTLIT
    | TCHARLIT
    | TSTRLIT
    /*other expressions*/
    | TLPAREN expr TRPAREN
    | TIDENT TLBRACK TINTLIT TRBRACK
    | TIDENT TLPAREN expr_list TRPAREN
    | prepost
    | unary

    /*arithmetic operators (blocked off by order of precedence)*/
    | expr TEXP expr

    | expr TMUL expr
    | expr TDIV expr
    | expr TMOD expr

    | expr TPLUS expr
    | expr TMIN expr

    | expr comparison expr

    | expr TEQ expr
    ;

unary: TNOT expr
     | TMIN expr %prec TUMIN
     ;

prepost: TIDENT TPLUSPLUS
       | TPLUSPLUS TIDENT
       | TIDENT TMINMIN
       | TMINMIN TIDENT
       ;

comparison: TLT
          | TLE
          | TGT
          | TGE
          | TEQEQ
          | TNE
          | TAND
          | TOR
          ;

optional_expr: /*nothing */
             | expr
             ;

expr_list: /*nothing*/
         | expr
         | expr_list TCOMMA expr
         ;
