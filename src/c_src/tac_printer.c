#include "tac_printer.h"
#include "factype_tac.h"
#include "tac_list.h"
#include <assert.h>
#include <stdio.h>

extern void yyerror(const char *);

static
void print_tac_entry(tac_entry *, int i);
static
char * get_C_operator(tac_op operator);
static
char * get_value(tac_value * value);

void print_tac(tac_node * tac_list){
	tac_node * iterator = tac_list;
	int i=0;
	while(iterator != NULL){
		putchar('\n');
		print_tac_entry(iterator->value, i);
		putchar('\n');
		++i;

		iterator = iterator->next;
	}
}
/********************************************
			PRIVATE FUNCTIONS
*********************************************/
static
void print_tac_entry(tac_entry * entry, int i){
	
	if(entry != NULL)
		printf("e%d)", i);
	printf(get_C_operator(entry->op));
	if(entry->arg0 != NULL)
		printf(get_value(entry->arg0));
	if(entry->arg1 != NULL)
		printf(get_value(entry->arg1));
}

static
char * get_C_operator(tac_op operator){
	switch(operator){
		case TAC_SUM: return "+"; break;
		case TAC_DIFF: return "-"; break;
		case TAC_MULT: return "*"; break;
		case TAC_DIV: return "/"; break;
		case TAC_NOT: return "!"; break;
		case TAC_AND: return "&&"; break;
		case TAC_OR: return "||"; break;
		case TAC_LT: return "<"; break;
		case TAC_GT: return ">"; break;
		case TAC_LEQ: return "<="; break;
		case TAC_GEQ: return ">="; break;
		case TAC_NEQ: return "!="; break;
		case TAC_PRINT: return "printf"; break;
		case TAC_ASSIGNMENT: return "="; break;
		/* the others are all unrecognized operators */
		case TAC_IMPLY: yyerror("TAC_Printer - IMPLY is not a valid operator"); break;
		case TAC_IFF: yyerror("TAC_Printer - IFF is not a valid operator"); break;
		case TAC_XOR: yyerror("TAC_Printer - XOR is not a valid operator"); break;
		case TAC_FRACT: yyerror("TAC_Printer - FRACT is not a valid operator"); break;
		case TAC_BOOL: yyerror("TAC_Printer - BOOL is not a valid operator"); break;
		case TAC_AOP1: yyerror("TAC_Printer - AOP1 is not a valid operator"); break;
		case TAC_AOP2: yyerror("TAC_Printer - AOP2 is not a valid operator"); break;
		case TAC_BOP1: yyerror("TAC_Printer - BOP1 is not a valid operator"); break;
		case TAC_BOP2: yyerror("TAC_Printer - BOP2 is not a valid operator"); break;
		case TAC_RELOP: yyerror("TAC_Printer - RELOP is not a valid operator"); break;
		case TAC_WHILE: yyerror("TAC_Printer - WHILE is not a valid operator"); break;
		case TAC_IF: yyerror("TAC_Printer - IF is not a valid operator"); break;
		case TAC_ELSE: yyerror("TAC_Printer - ELSE is not a valid operator"); break;
		default: yyerror("TAC_Printer - operator not recognized"); break;
	}
	return "";
}

static
char * get_value(tac_value * value){
	if(get_value == NULL)
		return "";
	char * buffer = "";
	if(value->address != NULL){
		if(value->address->value != NULL){
			buffer = "OK";
			return buffer;
		}
	}
	else if(value->instruction != NULL){
		buffer = calloc(20, sizeof(char));
		sprintf(buffer, "%p", value->instruction);
		return buffer;
	}
	
	return buffer;
}
