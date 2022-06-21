#include "asm.h"


int asmerror(char *erro) {
    fprintf(stderr, "%s\n", erro);
    return 1;
}

void assemble(FILE *source, FILE *output){
    fprintf(output, "v2.0 raw\n");
    asmlineno = 1;
    asmout = output;
    asmrestart(source);
    asmparse();
}