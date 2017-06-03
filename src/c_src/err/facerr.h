/**
 * @brief definition of fac_err codes and err_handler function. It deals with 
 * the errors encountered during the lexer phase.
 * @file
 * @author mirko.bez@studenti.unipd.it
 * @author stefano.munari.1@studenti.unipd.it
 */

#ifndef _FACERR_H_
#define _FACERR_H_
#include <stdio.h>
#include <errno.h>

typedef unsigned int uint;

extern char* yytext;
extern int yyleng;
extern int yylineno;

/** enumeration codes used in error handling to select the printing mode */
typedef enum err_input {
	FAC_STANDARD_ERROR,
	FAC_STRING,
	FAC_LINE,
	FAC_LINE_STRING,
} err_input;
/**
 * error handler function
 * @param err - the string to print
 * @param mode - the format for printing
 */
void err_handler(char* err, err_input mode);
#endif/*_FACERR_H_*/
