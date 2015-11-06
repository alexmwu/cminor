%{
/*C preamble*/
#include "scanner.yy.h"
#include "ast/decl.h"
#include "ast/stmt.h"
#include "ast/expr.h"

struct decl *programRoot;

//TODO: better error messages (e.g., line number and line of error)
void yyerror(const char *s) { fprintf(stderr, "PARSE_ERROR at line %d: %s\n", yylineno, s); }
%}

%error-verbose

%union {
  struct decl *decl;
  struct stmt *stmt;
  struct expr *expr;
  struct param_list *param_list;
  struct symbol *symbol;
  struct type *type;
  int token;
  long intLit;
  char *strLit;
  char charLit;
}

%token <token> TINT TSTR TCHAR TBOOL TARR TVOID
%token <token> TTRUE TFALSE TIF TELSE TWHILE TFOR
%token <token> TFUNC TRET TPRINT

%token <token> TSEMI TCOMMA TCOL TEQ
%token <token> TLBRACE TRBRACE TLPAREN TRPAREN TLBRACK TRBRACK

%token <intLit> TINTLIT
%token <strLit> TSTRLIT
%token <charLit> TCHARLIT
%token <strLit> TIDENT

%token <token> TPLUSPLUS TMINMIN TEXP TPLUS TMIN TMUL TDIV TMOD
%token <token> TLT TLE TGT TGE TEQEQ TNE TAND TOR TNOT


%type <decl> program decl_list decl
%type <param_list> optional_param_list param_list param
%type <stmt> stmt other_stmt matched unmatched optional_stmt_list stmt_list
%type <expr> expr optional_expr expr_list optional_expr_list assign_expr or_expr and_expr comparison_expr add_expr mul_expr exp_expr unary prepost group_arr_func atomic
%type <type> type

%start program

%%

/*
 *TODO: add last element for list types (allows constant
 *time linked-list access (useful for left recursion).
 */

/*Yacc returns (in $$) $1 by default*/
program: decl_list
       { programRoot = $1; }
       ;

decl_list: decl_list decl
            { struct decl *curr = $1; while(curr -> next) { curr = curr -> next; } curr -> next = $2; }
         | decl
         | /*nothing*/
            { $$ = 0; }
         ;

decl: TIDENT TCOL type TEQ expr TSEMI
      { $$ = decl_create(expr_create_name($1), $3, $5, 0, 0); }
    | TIDENT TCOL type TSEMI
      { $$ = decl_create(expr_create_name($1), $3, 0, 0, 0); }
    | TIDENT TCOL type TEQ TLBRACE expr_list TRBRACE TSEMI
      { $$ = decl_create(expr_create_name($1), $3, $6, 0, 0); }
    | TIDENT TCOL type TEQ TLBRACE optional_stmt_list TRBRACE
      { $$ = decl_create(expr_create_name($1), $3, 0, $6, 0); }
    ;

type: TSTR
      { $$ = type_create(TYPE_STRING, 0, 0, 0); }
    | TINT
      { $$ = type_create(TYPE_INTEGER, 0, 0, 0); }
    | TCHAR
      { $$ = type_create(TYPE_CHARACTER, 0, 0, 0); }
    | TBOOL
      { $$ = type_create(TYPE_BOOLEAN, 0, 0, 0); }
    | TARR TLBRACK TINTLIT TRBRACK type /*TODO: see if only fixed sized numbers (integer_literal). i.e., no expressions?*/
      { $$ = type_create(TYPE_ARRAY_DECL, 0, $5, expr_create_integer_literal($3)); }
      /*{ $$ = type_create(TYPE_ARR,*/
    | TARR TLBRACK TRBRACK type
      { $$ = type_create(TYPE_ARRAY, 0, $4, 0); }
    | TFUNC type TLPAREN optional_param_list TRPAREN
      { $$ = type_create(TYPE_FUNCTION, $4, $2, 0); }
    | TVOID
      { $$ = type_create(TYPE_VOID, 0, 0, 0); }
    ;

optional_param_list: /*nothing*/
                      { $$ = 0; }
          | param_list
          ;

param_list: param
          | param_list TCOMMA param
            { struct param_list *curr = $1; while(curr -> next) { curr = curr -> next; } curr -> next = $3; }
          ;

param: TIDENT TCOL type
        { $$ = param_list_create(expr_create_name($1), $3, 0); }
     ;

stmt: matched
    | unmatched
    ;

other_stmt: decl
            { $$ = stmt_create(STMT_DECL, $1, 0, 0, 0, 0, 0); }
          | expr TSEMI
            { $$ = stmt_create(STMT_EXPR, 0, 0, $1, 0, 0, 0); }
          /*FOR needs to be matched rather than stmt otherwise there will be ambiguities (e.g., if X for() if Y else Z; Z can match with Y or X) since stmt covers unmatched as well with the for loop as well*/
          | TFOR TLPAREN optional_expr TSEMI optional_expr TSEMI optional_expr TRPAREN matched
            { $$ = stmt_create(STMT_FOR, 0, $3, $5, $7, $9, 0); }
          | TLBRACE optional_stmt_list TRBRACE
            { $$ = stmt_create(STMT_BLOCK, 0, 0, 0, 0, $2, 0); }
          | TRET optional_expr TSEMI
            { $$ = stmt_create(STMT_RET, 0, 0, $2, 0, 0, 0); }
          | TPRINT optional_expr_list TSEMI
            { $$ = stmt_create(STMT_PRINT, 0, 0, $2, 0, 0, 0); }
          ;

matched: TIF TLPAREN expr TRPAREN matched TELSE matched
          { $$ = stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5, $7); }
       | other_stmt
       ;

unmatched: TIF TLPAREN expr TRPAREN stmt
            { $$ = stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5, 0); }
         | TIF TLPAREN expr TRPAREN matched TELSE unmatched
            { $$ = stmt_create(STMT_IF_ELSE, 0, 0, $3, 0, $5, $7); }
         ;

optional_stmt_list: /*nothing*/
                    { $$ = 0; }
                  | stmt_list
                  ;

stmt_list: stmt
         | stmt_list stmt
            { struct stmt *curr = $1; while(curr -> next) { curr = curr -> next; } curr -> next = $2; }
         ;

/*Everything from expr to expr_list is for expr; the many rules are for operator precedence*/
expr: assign_expr
    ;

assign_expr: TIDENT TEQ or_expr
              { $$ = expr_create(EXPR_EQ, expr_create_name($1), $3, 0); }
           | TIDENT TLBRACK expr TRBRACK TEQ or_expr
              { $$ = expr_create(EXPR_ARREQ, expr_create_name($1), $3, $6); }
           | or_expr
           ;

or_expr: or_expr TOR and_expr
          { $$ = expr_create(EXPR_OR, $1, $3, 0); }
       | and_expr
       ;

and_expr: and_expr TAND comparison_expr
          { $$ = expr_create(EXPR_AND, $1, $3, 0); }
        | comparison_expr
        ;

comparison_expr: comparison_expr TLT add_expr
                  { $$ = expr_create(EXPR_LT, $1, $3, 0); }
               | comparison_expr TLE add_expr
                  { $$ = expr_create(EXPR_LE, $1, $3, 0); }
               | comparison_expr TGT add_expr
                  { $$ = expr_create(EXPR_GT, $1, $3, 0); }
               | comparison_expr TGE add_expr
                  { $$ = expr_create(EXPR_GE, $1, $3, 0); }
               | comparison_expr TEQEQ add_expr
                  { $$ = expr_create(EXPR_EQEQ, $1, $3, 0); }
               | comparison_expr TNE add_expr
                  { $$ = expr_create(EXPR_NE, $1, $3, 0); }
               | add_expr
               ;

/*Also encapsulates minus*/
add_expr: add_expr TPLUS mul_expr
          { $$ = expr_create(EXPR_PLUS, $1, $3, 0); }
        | add_expr TMIN mul_expr
          { $$ = expr_create(EXPR_MIN, $1, $3, 0); }
        | mul_expr
        ;

/*Also encapsulates division and modulus*/
mul_expr: mul_expr TMUL exp_expr
          { $$ = expr_create(EXPR_MUL, $1, $3, 0); }
        | mul_expr TDIV exp_expr
          { $$ = expr_create(EXPR_DIV, $1, $3, 0); }
        | mul_expr TMOD exp_expr
          { $$ = expr_create(EXPR_MOD, $1, $3, 0); }
        | exp_expr
        ;

exp_expr: exp_expr TEXP unary
          { $$ = expr_create(EXPR_EXP, $1, $3, 0); }
        | unary
        ;


unary: TNOT unary
        { $$ = expr_create(EXPR_NOT, 0, $2, 0); }
     | TMIN unary
        { $$ = expr_create(EXPR_MIN, 0, $2, 0); }
     | prepost
     ;

prepost: TIDENT TPLUSPLUS
          { $$ = expr_create(EXPR_PLUSPLUS, expr_create_name($1), 0, 0); }
       | TPLUSPLUS TIDENT
          { $$ = expr_create(EXPR_PLUSPLUS, 0, expr_create_name($2), 0); }
       | TIDENT TMINMIN
          { $$ = expr_create(EXPR_MINMIN, expr_create_name($1), 0, 0); }
       | TMINMIN TIDENT
          { $$ = expr_create(EXPR_MINMIN, 0, expr_create_name($2), 0); }
       | group_arr_func
       ;

group_arr_func: TLPAREN expr TRPAREN
                { $$ = expr_create(EXPR_GROUP, 0, 0, $2); }
              | TIDENT TLBRACK expr TRBRACK
                { $$ = expr_create(EXPR_ARR, expr_create_name($1), $3, 0); }
              | TIDENT TLPAREN optional_expr_list TRPAREN
                { $$ = expr_create(EXPR_FUNC, expr_create_name($1), $3, 0); }
              | atomic
              ;

optional_expr: /*nothing */
                { $$ = 0; }
             | expr
             ;

expr_list: expr
         | expr_list TCOMMA expr
            { struct expr *curr = $1; while(curr -> next) { curr = curr -> next; } curr -> next = $3; }
         ;

optional_expr_list: /*nothing*/
                    { $$ = 0; }
                  | expr_list
                  ;

atomic: TTRUE
        { $$ = expr_create_boolean_literal(1); }
      | TFALSE
        { $$ = expr_create_boolean_literal(0); }
      | TINTLIT
        { $$ = expr_create_integer_literal($1); }
      | TCHARLIT
        { $$ = expr_create_character_literal($1); }
      | TSTRLIT
        { $$ = expr_create_string_literal($1); }
      | TIDENT
        { $$ = expr_create_name($1); }
      ;
