#include "as.h"
  
int main(int argc, char **argv) {
	FILE *f;
    
    if (argc<2) {
        fflush(stdout);
        fflush(stderr);
        printf("You must specify a source folder\n");
        printf("Usage:\ttang <source_file> <output_file>\n");
        printf("\ttang <source_file>   outputs to stdout\n");
        return 1;
    }
    if (argc==3) {
        yyout = fopen(argv[2], "w");
        //printf("path: %s\n", argv[2]);
    }
    if ((f=fopen(argv[1], "r"))) {
        compile(f); 
    } else {
        yyerror("File not found\n");
    }
	
	return 0;
}