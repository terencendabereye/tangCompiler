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

clean:
	trash *.tab.c *.tab.h *.yy.c
build:
	bison -d -o ./bin/asm.tab.c ./assembler/asm.y
	flex -o ./bin/lex.asm.y ./assembler/asm.l
	cc $(CCFLAGS) -c -I"./compiler" -I"./assembler" -I"./bin" -o ./bin/asmfunc.o ./assembler/asmfunc.c

	flex -o ./bin/lex.lblrep.c  ./labelReplacement/lr.l 

	bison -d -o ./bin/cmp.tab.c ./compiler/cmp.y
	flex -o ./bin/lex.cmp.c ./compiler/cmp.l 
	cc $(CCFLAGS) -I"./compiler" -I"./assembler" -c -o ./bin/main.o ./compiler/main.c
	cc $(CCFLAGS) -I"./compiler" -c -o ./bin/cmpfunc.o ./compiler/cmpfunc.c

	cc $(CCFLAGS) -I"./compiler" -I"./assembler" -I"./bin" -c ./bin/*.c
	mv *.o ./bin

	cc -o tang $(CCFLAGS) -I"./compiler/" -I"./assembler/" -I"./bin/" ./bin/*.o
#	cc -I"./compiler" -I"./assembler" -I"./bin" ./bin/*.c ./compiler/main.c