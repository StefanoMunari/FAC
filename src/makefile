PARSER_PATH = $(PWD)/parser
LEXER_PATH = $(PWD)/lexer
PARSER_FILE = parser
C_SOURCES = $(wildcard $(LEXER_PATH)/c_src/*.c)

all: compile

generate_parser:
	bison -d $(PARSER_PATH)/$(PARSER_FILE).y
	mv $(PARSER_FILE).tab.* $(LEXER_PATH)/c_src

generate_lexer: generate_parser
	@cd $(LEXER_PATH) && \
	python $(LEXER_PATH)/lexer.py

lex.yy.c: generate_lexer
	flex -o $(LEXER_PATH)/lex.yy.c $(LEXER_PATH)/lexer.fl

compile: lex.yy.c
	gcc -O3 -o fac $(C_SOURCES) $(LEXER_PATH)/lex.yy.c -lfl

run: compile
	./parser

clean:
	rm -f *.yy.c *.o *.out fac $(LEXER_PATH)/lex.yy.c $(LEXER_PATH)/c_src/$(PARSER_FILE).tab.*
