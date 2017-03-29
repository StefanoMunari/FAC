#include <stdbool.h>
int extract_exp(int pos){
	char exp[2];
	int j = 0;
	for(j=pos; j<yyleng; ++j)
		exp[j-pos]=yytext[j];
	return atoi(exp);
}
int precision(){
	int int_len=0, 
		precision=yyleng;
	bool complete=false;
	int i = 0;
	for(i=0; !complete && i<yyleng; ++i){
		switch(yytext[i]){
			case 'E'://exponent
				++i;//next char
				//remove integer part
				precision=i-int_len;
				if(yytext[i] == '-')//negative exponent
					precision=precision+extract_exp(i+1);
				complete=true;
				break;
			case '.'://fractional part
				++int_len;
				//remove integer part
				precision=precision-int_len;
				break;
			default://count integer chars
				++int_len;
		}
	}
	return precision;
}
