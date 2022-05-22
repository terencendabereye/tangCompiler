%option noyywrap yylineno
%{
    #include "as.h"
    #import "as.tab.h"
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
        yylval.i = newSym(yytext);
        return ID;
    }
\n      {}
[ \t]+  {}
"//".+  {}   /*handle line comments*/
.      {printf("unknown char '%c'\n", yytext[0]);}
%%