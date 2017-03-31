#ifndef _LEXER_H_
#define _LEXER_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

/** enumeration type used in error handling */
typedef enum _err_input {
	FAC_STANDARD_ERROR, 
	FAC_STRING, 
	FAC_LINE, 
	FAC_MULTIPLE,
} err_input;

typedef unsigned int uint;

extern char* yytext;
extern int yyleng;
extern uint line_counter;
/**
 * function called when a fract is recognized in order to extract numerator
 * and denumerator
 */
void tokenize_fract();
/**
 * error handler function, catch errors
 * @param the string to print
 * @param is the mode format for printing
 */
void err_handler(char*, err_input);
#endif/*_LEXER_H_*/
