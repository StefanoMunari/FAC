#ifndef _LEXER_H_
#define _LEXER_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef unsigned int uint;

extern char* yytext;
extern int yyleng;
extern uint line_counter;
/**
 * function called when a fract is recognized in order to extract numerator
 * and denumerator
 */
void tokenize_fract();
/*
 * error handler function, catch errors
 * first parameter is the string to print
 * second parameter is the mode format for printing
 */
void err_handler(char*, char);
#endif/*_LEXER_H_*/