#include "lexer.h"

long long gcd(long long u, long long v) {
	return (v != 0)?gcd(v, u%v):u;
}

void tokenize_fract(char * yytext, int yyleng){
	char * c = malloc(sizeof(char) * (yyleng+1));
	char * buf0 = malloc(sizeof(char) * (yyleng));
	char * buf1 = malloc(sizeof(char) * (yyleng));
	strncpy(c, yytext, yyleng*sizeof(char));
	c[yyleng] = '\0';
	sscanf(c,"[%[^|]|%[^]]]", buf0, buf1);
	
	long long numerator = strtol(buf0, NULL, 0);
	long long denominator = strtol(buf1, NULL, 0);
	long long g = gcd(numerator, denominator);
	numerator /= g;
	denominator /= g;
	printf("<FRACT,%lld|%lld>\n", numerator, denominator); 
	
	free(c);
	free(buf0);
	free(buf1);
}

