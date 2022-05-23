
%{
	#include "as.h"
%}

%union {
	int i;
	struct ast *tree;
}

%right '='
%left '+'
%left '-'
%left '^'
%left '&'
%left '|'
%nonassoc '!'
%nonassoc '('

%token <i> DEC HEX BIN ID
%type <i> ptr_x16
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
address: '@' '(' ptr_x16 ')'			{$$ = newTerminal(ptrSet, $3);}
| ID									{$$ = newTerminal(varSet, $1);}
;
reference: '@''@' '(' expression ')'	{$$ = newNode(addrRef, $4, NULL);}
| ID									{$$ = newTerminal(varRef, $1);}	
;
value: DEC 								{$$ = newTerminal(byteRaw, $1);}
| BIN									{$$ = newTerminal(byteRaw, $1);}
| HEX									{$$ = newTerminal(byteRaw, $1);}
;
ptr_x16: DEC							{$$ = $1;}
| BIN									{$$ = $1;}
| HEX									{$$ = $1;}
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
