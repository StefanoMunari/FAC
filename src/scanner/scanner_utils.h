#ifndef __SCANNER_UTILS__H__
#define __SCANNER_UTILS__H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/**
 * function called when a fract is recognized in order to extract numerator
 * and denumerator
 */
void tokenize_fract(char * yytext, int yyleng);
#endif /*#define __SCANNER_UTILS__H__*/
