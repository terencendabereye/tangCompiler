CCARGS:=-g -O0
.PHONY: build test bld_lib

asm: asm.h asm.l asm.y asmfunc.c main.c
	bison -d asm.y 
	flex asm.l
	clang $(CCARGS)lex.yy.c asm.tab.c asmfunc.c main.c -o asm

test: asm.h asm.l asm.y asmfunc.c main.c asm
	./asm -s test.txt

build: asm.h asm.l asm.y asmfunc.c main.c
	bison -d asm.y 
	flex asm.l
	clang $(CCARGS) lex.yy.c asm.tab.c asmfunc.c main.c -o asm

bld_lib: asm.h asm.l asm.y asmfunc.c
	bison -d asm.y 
	flex asm.l
	clang -c $(CCARGS)lex.yy.c asm.tab.c asmfunc.c
	ld -r lex.yy.o asm.tab.o asmfunc.o -o libasm
	trash *.o