#include "facerr.h"

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
			/* standard error means that errno was set,
			   therefore perror can be used */
			perror(err);
			break;
		default:
			fprintf(stderr, "%s\n", err);
	}
}
