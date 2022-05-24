TESTFILE:=test.txt
OUTFILE:=

.PHONY: test clean build all
all: compiler

tang: as.y as.l asfunc.c as.h
	bison -d as.y
	flex as.l
	cc -o $@ as.tab.c lex.yy.c asfunc.c

test: tang
	./tang $(TESTFILE) $(OUTFILE)

clean: *.tab.c *.tab.h *.yy.c
	trash $^ 
build:
	bison -d as.y
	flex as.l
	cc -o tang as.tab.c lex.yy.c asfunc.c