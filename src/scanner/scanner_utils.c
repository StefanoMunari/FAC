#include "scanner_utils.h"
void tokenize_fract(char * yytext, int yyleng){
	char * c = malloc(sizeof(char) * (yyleng+1));
	char * buf0 = malloc(sizeof(char) * (yyleng));
	char * buf1 = malloc(sizeof(char) * (yyleng));
	strncpy(c, yytext, yyleng*sizeof(char));
	c[yyleng] = '\0';
	sscanf(c,"[%[^|]|%[^]]]", buf0, buf1);
	
	long long numerator = strtoll(buf0, NULL, 0);
	long long denominator = strtoll(buf1, NULL, 0);
	printf("<FRACT, %lld, %lld>\n", numerator, denominator); 
	
	free(c);
	free(buf0);
	free(buf1);
}

