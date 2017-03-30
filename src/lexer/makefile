C_SOURCES = $(wildcard c/*.c)

all: run

generate: lexer.py
	python lexer.py

lex.yy.c: generate lexer.fl
	flex lexer.fl

compile: lex.yy.c
	gcc -o lexer $(C_SOURCES) lex.yy.c -lfl  

run: compile
	./lexer

clean:
	rm -f *.yy.c *.o *.out lexer lexer.fl