#ifndef _LEXER_H_
#define _LEXER_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/**
 * function called when a fract is recognized in order to extract numerator
 * and denumerator
 */
void tokenize_fract(char * yytext, int yyleng);
#endif/*_LEXER_H_*/