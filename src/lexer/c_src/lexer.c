#include "lexer.h"

long long gcd(long long x, long long y) {
	return (y != 0)?gcd(y, x%y):x;
}

void tokenize_fract(char * yytext, int yyleng){
	char fract[(yyleng+1)];
	char num_buf[yyleng];
	char den_buf[yyleng];
	strncpy(fract, yytext, yyleng*sizeof(char));
	fract[yyleng] = '\0';
	sscanf(fract,"[%[^|]|%[^]]]", num_buf, den_buf);
	
	long long numerator = strtol(num_buf, NULL, 0);
	long long denominator = strtol(den_buf, NULL, 0);
	long long g = gcd(numerator, denominator);
	numerator /= g;
	denominator /= g;
	printf("<FRACT,%lld|%lld>\n", numerator, denominator); 
}

