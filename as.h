#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VAR_LENGTH 30
#define VARS_START 60000
#define TEMP_VARS_START 50000

int yylex();
void yyerror(char *);
void yyrestart(FILE *);

struct ast{
    char nodeType;
    struct ast *l;
    struct ast *r;
};
struct terminalNode{
    char nodeType;
    int value;
};
struct symtab{
    char *name;
    int address;
    struct symtab *next;
};

struct ast *newNode(char type, struct ast* l, struct ast *r);
struct ast *newTerminal(char type, int value);
int eval(struct ast *node);
int newSym(char *);
int lookupSym(char *);