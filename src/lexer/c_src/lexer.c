#include "lexer.h"
/** "private" function that computes the gcd of two numbers 
 * @param y
 * @param x
 * @return gcd of x and y
 */
long long gcd(long long x, long long y) {
	return (y != 0)? gcd(y, x%y) : x;
}

fract_t tokenize_fract(){
	char fract[(++yyleng)];
	char num_buf[yyleng];
	char den_buf[yyleng];
	
	strncpy(fract, yytext, yyleng*sizeof(char));
	fract[yyleng] = '\0';
	sscanf(fract,"[%[^|]|%[^]]]", num_buf, den_buf);
	
	long long numerator = strtol(num_buf, NULL, 0);
	long long denumerator = strtol(den_buf, NULL, 0);
	long long g = gcd(numerator, denumerator);
	numerator /= g;
	denumerator /= g;
	fract_t f;
	f.num = (int) numerator;
	f.den = (int) denumerator;
	return f;
}

void err_handler(char* err, err_input mode){
	switch(mode){
		case FAC_STRING: /*string*/
			fprintf(stderr, err, yytext);
			break;
		case FAC_LINE: /*line*/ 
			fprintf(stderr, err, line_counter);
			break;
		case FAC_LINE_STRING: /*line + string*/ 
			fprintf(stderr, err, line_counter, yytext);
			break;
		case FAC_STANDARD_ERROR: 
			/* standard error means that errno was set, therefore perror can be used */
			perror(err);
			break;
		default: 
			fprintf(stderr, err);
	}
}
