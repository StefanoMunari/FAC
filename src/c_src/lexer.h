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


typedef unsigned int uint;

extern char* yytext;
extern int yyleng;
extern int yylineno;
extern YYLTYPE yylloc;
/**
 * function called when a fract is recognized in order to extract numerator
 * and denumerator
 */
fract_t tokenize_fract();

void setYYlloc();
#endif/*_LEXER_H_*/
