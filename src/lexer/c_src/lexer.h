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
extern char* yytext;
extern int yyleng;
void tokenize_fract();
#endif/*_LEXER_H_*/