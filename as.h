#ifndef _AS_H
#define _AS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VAR_LENGTH 30
#define VARS_START 10
#define TEMP_VARS_START 0

int yylex();
void yyerror(char *);
void yyrestart(FILE *);
int yyparse();
extern FILE *yyout;

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
    addrSet,
    varSet,
    ptrRef,
    ptrSet,
    assign,
    rawAsm,
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
int newSym(const char *, struct symtab *);
int lookupSym(const char *, struct symtab *);
int newVar(const char *);
int lookupVar(const char *);
int newLabel(const char *);
int lookupLabel(const char *);
void compile(FILE *);

struct symtab *varTable;
struct symtab *labelTable;

#endif