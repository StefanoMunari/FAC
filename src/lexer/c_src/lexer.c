#include "lexer.h"

long long gcd(long long x, long long y) {
	return (y != 0)?gcd(y, x%y):x;
}

void tokenize_fract(){
	char fract[(++yyleng)];
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

void err_handler(char* err, char mode){
	switch(mode){
		case 'S': /*string*/
			fprintf(stderr, err, yytext);
			break;
		case 'L': /*line*/ 
			fprintf(stderr, err, line_counter);
			break;
		case 'M': /*multiple: line + string*/ 
			fprintf(stderr, err, line_counter, yytext);
			break;
		default: 
			fprintf(stderr, err);
	}
}
