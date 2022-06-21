#include "asm.h"
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    char *sourcePath = NULL;
    char *outPath = NULL;
    FILE *output;
    FILE *input;
    char c;
    
    while ((c = getopt(argc, argv, "s:o:")) != -1) {
        switch (c)
        {
        case 's':
            sourcePath = optarg;
            break;
        case 'o':
            outPath = optarg;
            break;

        default:
            break;
        }
    }

    if (sourcePath == NULL){
        printf("must provide source file\n use arg -s");
    } else {
        if ((input = fopen(sourcePath, "r")) == NULL) {
            fprintf(stderr, "Source file does '%s' does not exist\n", sourcePath);
            exit(1);
        }
    }
    if (outPath == NULL) {
        output = stdout;
    } else {
        output = fopen(outPath, "w");
    }
    
    assemble(input, output);

    return 0;
}