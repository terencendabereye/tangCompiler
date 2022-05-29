TESTFILE:=test.txt
OUTFILE:=
CCFLAGS:= 

.PHONY: test clean build all
all: compiler

tang: cmp.y cmp.l cmpfunc.c cmp.h main.c
	bison -d cmp.y
	flex cmp.l
	flex ./labelReplacement/lr.l
	cc -o $@ $(CCFLAGS) cmp.tab.c lex.yy.c lex.lblrep.c cmpfunc.c main.c


	

test: tang cmp.y cmp.l cmpfunc.c cmp.h main.c
	./tang $(TESTFILE) $(OUTFILE)

clean: *.tab.c *.tab.h *.yy.c
	trash $^ 
build:

	bison -d ./assembler/asm.y 
	flex ./assembler/asm.l
	cc $(CCFLAGS) -c ./assembler/lex.asm.c ./assembler/asm.tab.c ./assembler/asmfunc.c 

	bison -d cmp.y
	flex cmp.l
	flex ./labelReplacement/lr.l
	cc -o tang $(CCFLAGS) cmp.tab.c lex.yy.c lex.lblrep.c cmpfunc.c main.c *.o