%{
/* C preamble */
%}

%token foo
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

type: STRING
    | INTEGER
    | CHAR
    | BOOLEAN
    | ARRAY /* TODO: need to see what Array type looks like */
    | FUNCTION type LEFT_PAREN param_list RIGHT_PAREN
    | VOID
    ;

param_list: /* nothing */
          | not_empty_param_list
          ;

/* TODO: not_empty_param_list */

param: IDENTIFIER COLON type
     ;

stmt: decl
    | expr SEMICOLON
    | FOR LEFT_PAREN optional_expr SEMICOLON optional_expr SEMICOLON optional_expr RIGHT_PAREN stmt
    | LBRACE stmt_list RBRACE
    /* TODO: fix the below dangling else */
    | IF LEFT_PAREN expr RIGHT_PAREN stmt ELSE stmt
    | IF LEFT_PAREN expr RIGHT_PAREN stmt
    ;

/* TODO: expr */
/* TODO: optional_expr */
