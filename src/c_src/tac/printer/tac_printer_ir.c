#include "tac_printer_ir.h"
#include "../../types/factype_tac.h"
#include <stdio.h>

extern void yyerror(const char *, ...);
static
void print_tac_entry(tac_entry *, int i);
static
char * get_operator(tac_op operator);
static
void print_value(tac_value * value);

void print_tac(tac_list * tlist){
	printf("======= IR: Print the 3AC =======\n");
	tac_node * iterator = tlist->first;
	int i=0;
	while(iterator != NULL){
		print_tac_entry(iterator->value, i);
		putchar('\n');
		++i;
		iterator = iterator->next;
	}
	printf("======= IR: Print completed =======\n");
}
/********************************************
			PRIVATE FUNCTIONS
*********************************************/
static
void print_tac_entry(tac_entry * entry, int i){
	if(entry == NULL)
		return;
	if(entry != NULL)
		printf("%p:\t", entry);
	printf(get_operator(entry->op));
	putchar('\t');
	print_value(entry->arg0);
	putchar('\t');
	print_value(entry->arg1);
}

static
char * get_operator(tac_op operator){
	switch(operator){
		case TAC_PLUS: return "PLUS"; break;
		case TAC_SUM: return "+"; break;
		case TAC_MINUS: return "MINUS"; break;
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
		case TAC_EQ: return "=="; break;
		case TAC_IFF: return "IFF"; break;
		case TAC_XOR: return "XOR"; break;
		case TAC_PRINT: return "printf"; break;
		case TAC_ASSIGNMENT: return "="; break;
		case TAC_GOTO: return "goto"; break;
		case TAC_LABEL: return "label"; break;
		/* the others are all unrecognized operators */
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
		default: yyerror("TAC_Printer - operator %d not recognized", operator); break;
	}
	return "";
}

static
void print_value(tac_value * value){
	if(value == NULL)
		printf("(NULL)");
	else if(value->address != NULL){
		if(value->address->id != NULL)
			printf("%s",value->address->id);
		else {
			yyerror("Entry in symbol table is not initialized");
			exit(EXIT_FAILURE);
		}
	}
	else if(value->instruction != NULL)
		printf("%p", value->instruction);
	else if(value->fract != NULL)
		printf("[%d|%d]", value->fract->num, value->fract->den);
	else
		printf("%s", value->boolean?"true":"false");
}
