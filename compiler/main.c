#include "cmp.h"
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
	FILE *f_src;
	FILE *f_out;
    int flag_src, flag_out;
    char *outpath;
    char *srcpath;
    char c;
    char tmpTemplate[] = "./tmp/tmp.XXXXXX";
    flag_out = flag_src = 0;
    
    while ((c=getopt(argc, argv, "s:o:")) !=  -1) {
        switch (c){
            case 'o':
                outpath = optarg;
                flag_out = 1;
                break;
            case 's':
                srcpath = optarg;
                flag_src = 1;
                break;
            case '?':
                fprintf(stderr, "option -%c not recognised\n", optopt);
                break;
            default:
                abort();
                break;
        }
    }

    //printf("%d\n", optind);

    if (flag_out) {
        f_out = fopen(outpath, "w");
    } else {
        f_out = stdout;
    }

    if (!flag_src && (argc > 1)) {
        srcpath = argv[optind];
        flag_src = 1;
    }

    if (!flag_src) {
        fprintf(stderr, "\e[91m>\e[m no source provided\n");
        exit(1);
    } else {
        if((f_src = fopen(srcpath, "r"))) flag_src = 1;
    }
    
    FILE *f_tmp;
    FILE *f_tmp1;
    
    mkdir("tmp", 0777); // makes temporary directory;
    
    f_tmp = fopen("./tmp/tmp0", "w");
    compile(f_src, f_tmp);
    fclose(f_src);
    fclose(f_tmp);

    f_tmp = fopen("./tmp/tmp0", "r");
    f_tmp1 = fopen("./tmp/tmp1", "w");
    replaceLabels(f_tmp, f_tmp1);
    fclose(f_tmp);
    fclose(f_tmp1);

    f_tmp1 = fopen("./tmp/tmp1", "r");
    assemble(f_tmp1, f_out);
    fclose(f_tmp1);

    fclose(f_out);
    fclose(f_src);

    
    	
	return 0;
}