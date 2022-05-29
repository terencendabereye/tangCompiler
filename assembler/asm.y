%{
    #include "asm.h"
%}

%union {
    int i;
    enum command c;
    enum reg r;
}

%token EOL
%token <i> NUM REG
%token <c> COM 

%%
program: 
| program instruction EOL
| program instruction
;
instruction: COM               {fprintf(yyout, "%.2x 00\n", $1);}
| COM NUM               {fprintf(yyout, "%.2x %.2x\n", $1, $2);}
| COM REG REG       {fprintf(yyout, "%.2x %.2x\n", $1, ($2<<4)|($3));}
;
%%

