PARSER_PATH = $(PWD)/parser
LEXER_PATH = $(PWD)/lexer
PARSER_FILE = parser
C_SRC_PATH = $(PWD)/c_src
C_SOURCES = $(wildcard $(PWD)/c_src/*.c)
BIN = bin

C_FLAG = -O3 -g -Wall -Werror


all: compile

generate_parser:
	bison -dv $(PARSER_PATH)/$(PARSER_FILE).y
	mv $(PARSER_FILE).tab.* $(C_SRC_PATH)

generate_lexer: generate_parser
	@cp $(LEXER_PATH)/lexer.py $(PWD) && \
	python $(PWD)/lexer.py && \
	rm $(PWD)/lexer.py && \
	cp $(LEXER_PATH)/lexer.fl $(C_SRC_PATH)

lex.yy.c: generate_lexer
	flex -o $(C_SRC_PATH)/lex.yy.c $(C_SRC_PATH)/lexer.fl && \
	rm $(C_SRC_PATH)/lexer.fl

compile: lex.yy.c
	gcc $(C_FLAG) -o $(BIN)/fac $(C_SOURCES) -lfl && \
	rm $(C_SRC_PATH)/lex.yy.c

run: compile
	./parser

clean:
	rm -rf *.yy.c *.o *.out *.output bin/fac $(LEXER_PATH)/lex.yy.c */*.tab.*
