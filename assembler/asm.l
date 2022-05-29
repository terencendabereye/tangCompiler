%option noyywrap case-insensitive
%option prefix="asm"
%{ 
    #include "asm.h"
    #include "asm.tab.h"
%}

%%
noop      {asmlval.c = noop; return COM;}
input     {asmlval.c = com_input; return COM;}
mov       {asmlval.c = mov;    return COM;}
compute   {asmlval.c = compute;return COM;}
output    {asmlval.c = output; return COM;}
jump      {asmlval.c = jump;   return COM;}
branch    {asmlval.c = branch_if; return COM;}
add       {asmlval.i = 0; return NUM;}
sub       {asmlval.i = 1; return NUM;}

gr[0-7]     {asmlval.r = atoi(&asmtext[2]); return REG;}
ra          {asmlval.r = ra; return REG;}
rusrin |
rx          {asmlval.r = rx_rusrin; return REG;}
rpc |
ry          {asmlval.r = rpc_ry; return REG;}
ro  |
ri          {asmlval.r = rin_ro; return REG;}
ad0         {asmlval.r = ad0; return REG;}
ad1         {asmlval.r = ad1; return REG;}
mem          {asmlval.r = rmem; return REG;}


[0-9]+      {asmlval.i = atoi(asmtext); return NUM;}
[ \t\r]+     {}
\n          {asmlineno++; return EOL;}
.          {printf("assembler: unknown character: '%c' at line %d\n", asmtext[0], asmlineno); yyterminate();}
%%