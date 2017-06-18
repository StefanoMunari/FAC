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

/** flex's variable that contains the last read token */
extern char* yytext;
/** flex's variable that contains the actual line number of the lexer */
extern int yylineno;

/** enumeration codes used in error handling to select the printing mode */
typedef enum err_input {
	/** error that exploits the perror utility */
	FAC_STANDARD_ERROR,
	/** error that should print yytext */
	FAC_STRING,
	/** error that should print ONLY yylineno */
	FAC_LINE,
	/** error that should print both yylineno and yytext */
	FAC_LINE_STRING,
} err_input;
/**
 * error handler function
 * @param err - the string to print
 * @param mode - the format for printing
 */
void err_handler(char* err, err_input mode);
#endif/*_FACERR_H_*/
