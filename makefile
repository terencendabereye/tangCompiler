TESTFILE:=test.txt

.PHONY: test clean build all
all: compiler

compiler: as.y as.l asfunc.c as.h
	bison -d as.y
	flex as.l
	cc -o $@ as.tab.c lex.yy.c asfunc.c

test: compiler
	./compiler $(TESTFILE)

clean: *.tab.c *.tab.h *.yy.c
	trash $^ 
build:
	bison -d as.y
	flex as.l
	cc -o compiler as.tab.c lex.yy.c asfunc.c