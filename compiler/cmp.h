#ifndef _AS_H
#define _AS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VAR_LENGTH 30
#define VARS_START 0
#define TEMP_VARS_START 0
#define FN_STACK_0 0xff10
#define FN_STACK_1 0xff11

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
    jmp,
    jmplbl,
    byteout,
    bytein,
    labelSet,
    labelReplace,
    branch,
    eql,
    lsn,
    grt,
    fnCall,
    fnReturn,
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
struct ifNode{
    enum nodeType nodeType;
    struct ast *l;
    struct ast *r;
    struct ast *target;
};
struct symtab{
    char *name;
    int address;
    struct symtab *next;
};

struct ast *newNode(enum nodeType type, struct ast* l, struct ast *r);
struct ast *newTerminal(enum nodeType type, int value);
struct ast *newIfNode(enum nodeType type, struct ast *l, struct ast *r, struct ast *target);
int eval(struct ast *node);
int newSym(const char *, struct symtab *);
int lookupSym(const char *, struct symtab *);
void compile(FILE *in, FILE *out);
void replaceLabels(FILE *in, FILE *out);
void assemble(FILE *source, FILE *output);

struct symtab *varTable;
struct symtab *labelTable;

#endif