%{
/*C preamble*/
#include "scanner.yy.h"
//TODO: better error messages (e.g., line number and line of error)
void yyerror(const char *s) { printf("PARSE_ERROR: %s\n", s); }
%}

%union {
  int token;
  long intLit;
  char *strLit;
  char charLit;
  char *ident;
}

%token TINT TSTR TCHAR TBOOL TARR TVOID
%token TTRUE TFALSE TIF TELSE TWHILE TFOR
%token TFUNC TRET TPRINT

%token TSEMI TCOMMA TCOL TEQ
%token TLBRACE TRBRACE TLPAREN TRPAREN TLBRACK TRBRACK

%token TINTLIT TSTRLIT TCHARLIT
%token TIDENT

%token TPLUSPLUS TMINMIN TEXP TPLUS TMIN TMUL TDIV TMOD
%token TLT TLE TGT TGE TEQEQ TNE TAND TOR TNOT

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

stmt: matched
    | unmatched
    ;

other_stmt: decl
          | expr TSEMI
          | TFOR TLPAREN optional_expr TSEMI optional_expr TSEMI optional_expr TRPAREN stmt
          | TLBRACE stmt_list TRBRACE
          | TLBRACE TRBRACE
          | TRET optional_expr TSEMI
          | TPRINT expr_list TSEMI
          ;

matched: TIF TLPAREN expr TRPAREN matched TELSE matched
       | other_stmt
       ;

unmatched: TIF TLPAREN expr TRPAREN stmt
         | TIF TLPAREN expr TRPAREN matched TELSE unmatched
         ;

stmt_list: stmt
         | stmt_list stmt
         ;

/*Everything from expr to expr_list is for expr; the many rules are for operator precedence*/
expr: assign_expr
    ;

assign_expr: TIDENT TEQ or_expr
           ;

or_expr: or_expr TOR and_expr
       | and_expr
       ;

and_expr: and_expr TAND comparison_expr
        | comparison_expr
        ;

comparison_expr: comparison_expr TLT add_expr
               | comparison_expr TLE add_expr
               | comparison_expr TGT add_expr
               | comparison_expr TGE add_expr
               | comparison_expr TEQEQ add_expr
               | comparison_expr TNE add_expr
               | add_expr
               ;

/*Also encapsulates minus*/
add_expr: add_expr TPLUS mul_expr
        | add_expr TMIN mul_expr
        | mul_expr
        ;

/*Also encapsulates division and modulus*/
mul_expr: mul_expr TMUL exp_expr
        | mul_expr TDIV exp_expr
        | mul_expr TMOD exp_expr
        | exp_expr
        ;

exp_expr: exp_expr TEXP unary
        | unary
        ;


unary: TNOT unary
     | TMIN unary
     | prepost
     ;

prepost: TIDENT TPLUSPLUS
       | TPLUSPLUS TIDENT
       | TIDENT TMINMIN
       | TMINMIN TIDENT
       | group_arr_func
       ;

group_arr_func: TLPAREN expr TRPAREN
              | TIDENT TLBRACK TINTLIT TRBRACK
              | TIDENT TLPAREN expr_list TRPAREN
              | atomic
              ;

optional_expr: /*nothing */
             | expr
             ;

expr_list: /*nothing*/
         | expr
         | expr_list TCOMMA expr
         ;

atomic: TTRUE
      | TFALSE
      | TINTLIT
      | TCHARLIT
      | TSTRLIT
      ;
