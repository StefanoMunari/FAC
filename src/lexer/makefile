C_SOURCES = $(wildcard c_src/*.c)

all: compile

generate: lexer.py
	python lexer.py

lex.yy.c: generate lexer.fl
	flex lexer.fl

compile: lex.yy.c
	gcc -O3 -o lexer $(C_SOURCES) y.tab.c lex.yy.c -lfl  

run: compile
	./lexer

clean:
	rm -f *.yy.c *.o *.out lexer lexer.fl
