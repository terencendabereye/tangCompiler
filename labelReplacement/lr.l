%option yylineno noyywrap
%option prefix="lblrep"
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #define MAX_NUM_LABEL 50

    struct labelList{
        int index;
        int ad0;
        int ad1;
    };
    struct label{
        int index;
        int lineNum;
    };
    void newLbl(int index, int lineNum);
    int getLb_a0(int index);
    int getLb_a1(int index);
    void replaceLabels(FILE *, FILE *);

    struct labelList labels[MAX_NUM_LABEL] = {0};
%}

%%
"<"[0-9]+">"    {
    int index;
    int adr;
    char tmpNum[10];
    for (int i=1; i<(yyleng-1); i++){
        tmpNum[i-1] = yytext[i];
    }
    tmpNum[yyleng-1] = '\0';
    index = atoi(tmpNum);
    adr = (yylineno-1)*2;
    newLbl(index, adr);
    //fprintf(yyout, "index: %d, line %d\n", index, adr);

}
"<"[0-9]+,[0-9]">" {
    int index;
    int adr;
    char *tmpNum = yytext;
    tmpNum = strtok(tmpNum, "<"); // 1,0>
    tmpNum = strtok(tmpNum, ","); // 1
        index = atoi(tmpNum);
        //fprintf(yyout, "index: %d ", index);
    tmpNum = strtok(NULL, ","); // 0>
    tmpNum = strtok(tmpNum, ">"); // 0
        adr = atoi(tmpNum);
        //fprintf(yyout, "adr: %d ", adr);
    if (adr==0){
        fprintf(yyout, "%d", getLb_a0(index));
    } else if (adr==1) {
        fprintf(yyout, "%d", getLb_a1(index));
    }
}
\n {fprintf(yyout, "%c", yytext[0]);}
. {fprintf(yyout, "%c", yytext[0]);}
%%



void newLbl(int index, int lineNum){
    labels[index].ad0 = lineNum & 0xff;
    labels[index].ad1 = (lineNum & 0xff00)>>8;
}

int getLb_a0(int index){
    return labels[index].ad0;
}
int getLb_a1(int index){
    return labels[index].ad1;
}

void replaceLabels(FILE *in, FILE *out){   //like main()
    yyout = out;
    yyrestart(in);
    yylex();
}