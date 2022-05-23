
%{
	#include "as.h"
%}

%union {
	int i;
	struct ast *tree;
}

%left '='
%left '+'
%left '-'
%left '^'
%left '&'
%left '|'
%nonassoc '!'

%token <i> DEC HEX BIN ID

%type <tree> statement expression value address reference 

%%
program:
| program statement 					{eval($2);}
;
statement: address '=' expression ';'	{$$ = newNode(assign, $1, $3);}	
;
expression: value						{$$ = $1;}
| reference								{$$ = $1;}
| '(' expression ')'					{$$ = $2;}
| expression '+' expression				{$$ = newNode(addByte, $1, $3);}
| expression '-' expression				{$$ = newNode(subByte, $1, $3);}
| expression '&' expression				{$$ = newNode(bitAnd, $1, $3);}
| expression '|' expression				{$$ = newNode(bitOr, $1, $3);}
| expression '^' expression				{$$ = newNode(bitXor, $1, $3);}
| '!' expression						{$$ = newNode(bitNot, $2, NULL);}
;
address: '@' '(' expression ')'			{$$ = newNode('@', $3, NULL);}
| ID									{$$ = newTerminal(varRef, $1);}
;
reference: '@''@' '(' expression ')'	{$$ = newNode(addrRef, $4, NULL);}
| ID									{$$ = newTerminal(varRef, $1);}	
;
value: DEC 								{$$ = newTerminal(byteRaw, $1);}
| BIN									{$$ = newTerminal(byteRaw, $1);}
| HEX									{$$ = newTerminal(byteRaw, $1);}
;
%% 

int main(int argc, char **argv) {
	FILE *f;
	if (argc>1) {
		if ((f=fopen(argv[1], "r"))) {
			yyrestart(f);
			yyparse();
			fclose(f); 
		}
	}
	
	return 0;
}
void yyerror(char *a) {
	fprintf(stderr, "%s", a);
}