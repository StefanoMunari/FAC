#ifndef _LEXER_H_
#define _LEXER_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "facerr.h"
#include "factype.h"

typedef unsigned int uint;

extern char* yytext;
extern int yyleng;
extern uint line_counter;
/**
 * function called when a fract is recognized in order to extract numerator
 * and denumerator
 */
fract tokenize_fract();
/**
 * error handler function, catch errors
 * @param the string to print
 * @param is the mode format for printing
 */
void err_handler(char*, err_input);
#endif/*_LEXER_H_*/
