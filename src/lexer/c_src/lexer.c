#include "lexer.h"
#include "facmath.h"
fract_t tokenize_fract(){
	char fract[(++yyleng)];
	char num_buf[yyleng];
	char den_buf[yyleng];
	
	strncpy(fract, yytext, yyleng*sizeof(char));
	fract[yyleng] = '\0';
	sscanf(fract,"[%[^|]|%[^]]]", num_buf, den_buf);
	
	int numerator = (int) strtol(num_buf, NULL, 0);
	int denumerator = (int) strtol(den_buf, NULL, 0);
	
	fract_t f;
	f.num = numerator;
	f.den = denumerator;
	return normalizeFract(f);
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
