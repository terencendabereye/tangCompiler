%option noyywrap case-insensitive
%{ 
    #include "asm.h"
    #include "asm.tab.h"
%}

%%
noop      {yylval.c = noop; return COM;}
input     {yylval.c = com_input; return COM;}
mov       {yylval.c = mov;    return COM;}
compute   {yylval.c = compute;return COM;}
output    {yylval.c = output; return COM;}
jump      {yylval.c = jump;   return COM;}
branch    {yylval.c = branch_if; return COM;}
add       {yylval.i = 0; return NUM;}
sub       {yylval.i = 1; return NUM;}

gr[0-7]     {yylval.r = atoi(&yytext[2]); return REG;}
ra          {yylval.r = ra; return REG;}
rusrin |
rx          {yylval.r = rx_rusrin; return REG;}
rpc |
ry          {yylval.r = rpc_ry; return REG;}
ro  |
ri          {yylval.r = rin_ro; return REG;}
ad0         {yylval.r = ad0; return REG;}
ad1         {yylval.r = ad1; return REG;}
mem          {yylval.r = rmem; return REG;}


[0-9]+      {yylval.i = atoi(yytext); return NUM;}
[ \t\r]+     {}
\n          {yylineno++; return EOL;}
.          {printf("assembler: unknown character: '%c' at line %d\n", yytext[0], yylineno); yyterminate();}
%%