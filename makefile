# Provide arguments to the executable using the ARGS variable
# Provide arguments to the memory debugger/profiler using the VGOPTS variable
PARSER_PATH = $(PWD)/src/parser
LEXER_PATH = $(PWD)/src/lexer
C_SRC_PATH = $(PWD)/src/c_src
BIN_PATH = $(PWD)/bin
PARSER_FILE = parser
C_SOURCES = $(shell find $(C_SRC_PATH) -name *.c)
BIN=fac
VALGRIND=valgrind

C_FLAG = -O3 -g -Wall -Werror

all: compile

generate_parser:
	@echo "==Bison=="
	bison -d $(PARSER_PATH)/$(PARSER_FILE).y
	mv $(PARSER_FILE).tab.* $(C_SRC_PATH)
	@echo "========"

generate_lexer: generate_parser
	@cp $(LEXER_PATH)/lexer.py $(PWD) && \
	python $(PWD)/lexer.py && \
	rm $(PWD)/lexer.py && \
	cp $(LEXER_PATH)/lexer.fl $(C_SRC_PATH)

lex.yy.c: generate_lexer
	@echo "==Flex=="
	flex -o $(C_SRC_PATH)/lex.yy.c $(C_SRC_PATH)/lexer.fl && \
	rm $(C_SRC_PATH)/lexer.fl
	@echo "========"

compile: lex.yy.c
	@echo "==Compiling=="
	gcc $(C_FLAG) -o $(BIN_PATH)/$(BIN) $(C_SOURCES) -lfl && \
	rm $(C_SRC_PATH)/lex.yy.c
	@echo "========"

.PHONY: run
run: compile
	@echo "==Running=="
	$(BIN_PATH)/$(BIN) $(ARGS)
	@echo "========"

.PHONY: run
profile: compile
	@echo "==Profiling=="
	$(VALGRIND) $(VGOPTS) $(BIN_PATH)/$(BIN) $(ARGS)
	@echo "========"

.PHONY: doc
doc:
	@echo "==Generating documentation=="
	doxygen doxyfile
	@echo "========"

.PHONY: clean
clean:
	rm -rf *.yy.c *.o *.out *.output bin/fac $(LEXER_PATH)/lex.yy.c */*.tab.*
