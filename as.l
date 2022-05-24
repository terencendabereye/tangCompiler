%option noyywrap yylineno
%{
    #include "as.h"
    #import "as.tab.h"
    void yyterminate();
%}

%%
"@" |
":" |
"=" |
";" |
"(" |
")" |
"{" |
"+" |
"-" |
"&" |
"|" |
"!" |
"^" |
"}" {return yytext[0];}
[0-9]+  {yylval.i = atoi(yytext); return DEC;}
0x[0-9a-fA-F]+ {char *ptr; yylval.i = strtol(yytext, &ptr, 16);return HEX;}
0b[0-9a-fA-F]+ {
        char *ptr; 
        char *out = malloc(sizeof(char) * yyleng);
        for (int i=2; i<=yyleng; i++) {
            out[i-2] = yytext[i];
        }
        yylval.i = strtol(out, &ptr, 2);
        free(out);
        return BIN;
    }
[A-Za-z][A-Za-z0-9]*    {
        if (lookupSym(yytext)==-1){
            yylval.i = newSym(yytext);
            //printf("new var********* %s *\n", yytext);
        } else {
            yylval.i = lookupSym(yytext);
            //printf("existing var********** %s *\n", yytext);
        }
        return ID;
    }
$$.*    {
            //printf("%s\n", yytext);
            for (int i=2; i<yyleng; i++) {
                fprintf(yyout, "%c", yytext[i]);
            }
            fprintf(yyout, "\n");
        }
\n      {}
[ \t]+  {}
"//".+  {}   /*handle line comments*/
.       {
            fprintf(yyout, "unknown char '%c' at line: %d\n", yytext[0], yylineno);
            yyterminate();
        }
%%

void yyerror(char *a) {
	fprintf(stdout, "%s at line: %d", a, yylineno);
}

