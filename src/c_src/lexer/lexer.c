#include "lexer.h"
#include "../utils/facmath.h"

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

static
void setYYllocLine() {
	yylloc.first_line = yylloc.last_line = yylineno;
}

static
void setYYllocCol() {
	yylloc.first_column = yylloc.last_column;
	yylloc.last_column = yylloc.first_column + strlen(yytext);
}

void setYYlloc(){
	setYYllocLine();
	setYYllocCol();
}