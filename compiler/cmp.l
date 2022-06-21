%option noyywrap yylineno

%{
    #include "cmp.h"
    #include "cmp.tab.h"
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
"<" |
">" |
"}" {return yytext[0];}
goto {return JMP;}
byteout {return BYTEOUT;}
bytein {return BYTEIN;}
return {return RETURN;}
if {return IF;}
[0-9]+  {yylval.i = atoi(yytext); return DEC;}
'[ -~]'   {yylval.i = yytext[1]; return DEC;}
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
"var "[A-Za-z][A-Za-z0-9]* {
        char input[40];
        strcpy(input, yytext);
        char *tok = strtok(input, " ");
        tok = strtok(NULL, " ");
        //printf("%s\n", tok);
        if (lookupSym(tok, varTable)==-1){
            newSym(tok, varTable);
        } else {
            printf("\e[1;31mtang-error\e[m at line %d: can't reinitialize variable, '%s'\n" ,yylineno , tok);
            exit(0);
        }
        return VAR_DECL;
}
[A-Za-z][A-Za-z0-9]*    {
        if (lookupSym(yytext, varTable)==-1){
            //yylval.i = newSym(yytext ,varTable);
            printf("\e[1;31mtang-error\e[m at line %d: use of undefined symbol, '%s'\n" ,yylineno , yytext);
            exit(0);
            //printf("new var********* %s *\n", yytext);
        } else {
            yylval.i = lookupSym(yytext, varTable);
            //printf("existing var********** %s *\n", yytext);
        }
        return ID;
    } 
"."[A-Za-z][A-Za-z0-9]+    {
        if(lookupSym(yytext, labelTable)==-1){
            yylval.i = newSym(yytext, labelTable);
        } else {
            yylval.i = lookupSym(yytext, labelTable);
        }
        return LABEL;
    }
$.*    {
            //printf("%s\n", yytext);
            for (int i=1; i<yyleng; i++) {
                fprintf(yyout, "%c", yytext[i]);
            }
            fprintf(yyout, "\n");
        }

\n      {}
[ \t]+  {}
"//".+  {}   /*handle line comments*/
.       {
            fprintf(yyout, "compiler: unknown char '%c' at line: %d\n", yytext[0], yylineno);
            yyterminate();
        }
%%

void yyerror(char *a) {
	fprintf(stdout, "%s at line: %d\n", a, yylineno);
}

