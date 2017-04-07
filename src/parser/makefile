#C_SOURCES = $(wildcard c_src/*.c)
all: compile

#generate: parser.y
#	python parser.py

y.tab.c: #generate parser.y
	bison -d parser.y

#compile: y.tab.c
#	gcc -O3 -o parser 

#run: compile
#	./parser

clean:
	rm -f *.yy.c *.o *.out parser #parser.y
