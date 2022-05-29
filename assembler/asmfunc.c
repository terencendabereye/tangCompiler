#include "asm.h"


int yyerror(char *erro) {
    fprintf(stderr, "%s\n", erro);
    return 1;
}

void assemble(FILE *source, FILE *output){
    fprintf(output, "v2.0 raw\n");
    yylineno = 1;
    yyout = output;
    yyrestart(source);
    yyparse();
}