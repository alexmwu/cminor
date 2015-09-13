%token ID
%%
program:    /* Nothing */
    |       program ID
    ;
%%