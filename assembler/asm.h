#include <stdio.h>

//FILE *yyout; 
int asmlex();
int asmerror(char *);
int asmparse();
void asmrestart(FILE *);
void assemble(FILE *source, FILE *output);

extern int asmlineno;
extern FILE *asmout;

enum command{
    noop=0,
    com_input=1,
    mov=2,
    compute=3,
    output=4,
    jump=05,
    branch_if=6,
    getprogctr=7,       //in testing
    retsubroutine=8,    //is brocken
};
enum reg{
    gp0=0,          //general purpose registers gp0-gp7
    gp1,
    gp2,
    gp3,
    gp4,
    gp5,
    gp6,
    gp7,
    ra,             //results of apu here(read-only)
    rx_rusrin,          // apu input x (write-only), read from userinput buffer
    rpc_ry,         // program counter stored here on request, type bytes in buffer(read-only), apu input y (write-only)
    rin_ro,         //inout registers
    ad0,            //mem address lower
    ad1,            //mem address upper
    rmem,           //mem cache
};

