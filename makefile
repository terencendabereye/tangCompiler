TESTFILE:=test.txt
OUTFILE:=
CCFLAGS:= -g

.PHONY: test clean build all
all: compiler

tang: as.y as.l asfunc.c as.h main.c
	bison -d as.y
	flex as.l
	cc -o $@ $(CCFLAGS) as.tab.c lex.yy.c asfunc.c main.c

test: tang
	./tang $(TESTFILE) $(OUTFILE)

clean: *.tab.c *.tab.h *.yy.c
	trash $^ 
build:
	bison -d as.y
	flex as.l
	cc -o tang $(CCFLAGS) as.tab.c lex.yy.c asfunc.c main.c