TESTFILE:=test.txt
OUTFILE:=
CCFLAGS:= -g

.PHONY: test clean build all
all: compiler

tang: cmp.y cmp.l cmpfunc.c cmp.h main.c
	bison -d cmp.y
	flex cmp.l
	cc -o $@ $(CCFLAGS) cmp.tab.c lex.yy.c cmpfunc.c main.c

test: tang cmp.y cmp.l cmpfunc.c cmp.h main.c
	./tang $(TESTFILE) $(OUTFILE)

clean: *.tab.c *.tab.h *.yy.c
	trash $^ 
build:
	bison -d cmp.y
	flex cmp.l
	cc -o tang $(CCFLAGS) cmp.tab.c lex.yy.c cmpfunc.c main.c