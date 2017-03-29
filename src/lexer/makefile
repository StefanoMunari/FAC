all: run

lex.yy.c: lexer.fl
	flex lexer.fl

compile: lex.yy.c lexer.c
	gcc -o lexer lexer.c lex.yy.c -lfl  

run: compile
	./lexer

#generate: lexer.py
#	python lexer.py

clean:
	rm -f *.yy.c *.o *.out lexer