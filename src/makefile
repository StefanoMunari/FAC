C_SOURCES = $(wildcard c_src/*.c)
PARSER_SRC = $(pwd)/parser
LEXER_SRC = $(pwd)/lexer
PARSER_FILE = parser

all: compile

generate_parser:
	@cd $(PARSER_SRC)	
	bison -d $(PARSER_FILE).y
	mv $(PARSER_FILE).tab.* $(LEXER_SRC)/c_src
	@cd $(PWD)

generate_lexer: generate_parser
	@cd $(LEXER_SRC)
	python lexer.py
	@cd $(PWD)

compile: generate_lexer
	@cd $(LEXER_SRC)
	gcc -O3 -o parser $(C_SOURCES) lex.yy.c -lfl
	@mv parser $(PWD)

run: compile
	./parser

clean:
	rm -f *.yy.c *.o *.out parser
