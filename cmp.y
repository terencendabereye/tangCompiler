
%{
	#include "cmp.h"
%}

%union {
	int i;
	struct ast *tree;
	char *s;
}

%right '='
%left '+'
%left '-'
%left '^'
%left '&'
%left '|'
%nonassoc '!'
%nonassoc '('

%token JMP BYTEOUT IF
%token <i> DEC HEX BIN ID LABEL
%type <i> ptr_x16
%type <tree> statement expression value address reference label

%%
program:
| program statement 					{eval($2);}
;
statement: address '=' expression ';'	{$$ = newNode(assign, $1, $3);}	
| JMP '(' address ')' ';'				{$$ = newNode(jmp, $3, NULL);}
| JMP '(' LABEL ')' ';'					{$$ = newTerminal(jmplbl, $3);}
| IF expression label ';'		{$$ = newNode(branch, $2, $3)}
| BYTEOUT '(' expression ')' ';'		{$$ = newNode(byteout, $3, NULL);}
| LABEL ';'								{$$ = newTerminal(labelSet, $1);}
;
expression: value						{$$ = $1;}
| reference								{$$ = $1;}
| '(' expression ')'					{$$ = $2;}
| expression '+' expression				{$$ = newNode(addByte, $1, $3);}
| expression '-' expression				{$$ = newNode(subByte, $1, $3);}
| expression '&' expression				{$$ = newNode(bitAnd, $1, $3);}
| expression '|' expression				{$$ = newNode(bitOr, $1, $3);}
| expression '^' expression				{$$ = newNode(bitXor, $1, $3);}
;
label: LABEL 							{$$ = newTerminal(labelReplace, $1);}
address: '@' '(' ptr_x16 ')'			{$$ = newTerminal(ptrSet, $3);}
| ID									{$$ = newTerminal(varSet, $1);}
;
reference: '@''@' '(' ptr_x16 ')'	{$$ = newTerminal(addrRef, $4);}
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

