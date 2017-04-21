#ifndef _FACERR_H_
#define _FACERR_H_
#include <stdio.h>
#include <errno.h>

typedef unsigned int uint;

extern char* yytext;
extern int yyleng;
extern uint line_counter;

/** enumeration type used in error handling */
typedef enum _err_input {
	FAC_STANDARD_ERROR, 
	FAC_STRING, 
	FAC_LINE, 
	FAC_LINE_STRING,
} err_input;
/**
 * error handler function, catch errors
 * @param the string to print
 * @param is the mode format for printing
 */
void err_handler(char*, err_input);
#endif/*_FACERR_H_*/