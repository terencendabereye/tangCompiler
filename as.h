#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VAR_LENGTH 30
#define VARS_START 60000
#define TEMP_VARS_START 50000

int yylex();
void yyerror(char *);
void yyrestart(FILE *);

enum nodeType{
    addByte,
    subByte,
    bitAnd,
    bitOr,
    bitXor,
    bitNot,
    byteRaw,
    addrRef,
    varRef,
    assign,
    addr
};
struct ast{
    enum nodeType nodeType;
    struct ast *l;
    struct ast *r;
};
struct terminalNode{
    enum nodeType nodeType;
    int value;
};
struct symtab{
    char *name;
    int address;
    struct symtab *next;
};

struct ast *newNode(enum nodeType type, struct ast* l, struct ast *r);
struct ast *newTerminal(enum nodeType type, int value);
int eval(struct ast *node);
int newSym(char *);
int lookupSym(char *);