/**
 * @file
 * @brief file describing help functions of the lexer, called by the flex 
 * generated program
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
#ifndef _LEXER_H_
#define _LEXER_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "facerr.h"
#include "factype.h"
#include "ast.h"
#include "seq_tree.h"
#include "parser.tab.h"



extern char* yytext;
extern int yyleng;
extern int yylineno;
extern YYLTYPE yylloc;
/**
 * function called when a fract is recognized in order to extract numerator
 * and denumerator
 */
fract_t tokenize_fract();

/**
 * function called whenever a token is recognized. It sets the vaiable yylloc
 * with the information: first_column, last_column, first_row and last_row.
 */
void setYYlloc();
#endif/*_LEXER_H_*/
