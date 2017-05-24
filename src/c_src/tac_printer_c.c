
#include "tac_printer.h"
#include <stdio.h>
static int tcounter = 0;

static
void print_tac_entry(tac_node *);

static
char * get_operator(tac_op operator);

static
char * getValue(tac_value * tvalue, char * num_or_den);

static 
char * getBooleanValue(tac_value * tvalue);

char buffer[256];

void print_tac_c(tac_list * tlist){
	printf("=== C: Print the 3AC ===\n");
	printf("#include<stdio.h>\n");
	printf("int MCD(int u, int v) {\n");
	printf("\treturn (v != 0)?MCD(v, u %c v):u;\n}\n", 37);

	printf("int main(void){\n");

	
	int i=0;
	tcounter = 0;
	tac_node * iterator = tlist->first;
	while(iterator != NULL){
		print_tac_entry(iterator);
		++i;
		iterator = iterator->next;
	}
	printf("return 0;\n");
	printf("}\n");
	printf("=== C: End the 3AC ===\n");
}



void print_tac_entry(tac_node * node){
	tac_entry * entry = node->value;
	if(entry == NULL)
		return;
	
	//int tcounter = 0;
	switch(entry->op){
		case TAC_ASSIGNMENT: 
		{
			char * id = entry->arg0->address->id;
			switch(getType(id)){
				case BOOL_T:
				{
					char * boolean_value = getBooleanValue(entry->arg1);
					printf("%s = %s;\n", id, boolean_value);
					break;
				}
				case FRACT_T:
				{
					char * fract_val_num = getValue(entry->arg1, "num");
					char * fract_val_den = getValue(entry->arg1, "den");
					printf("%snum = %s;\n", id, fract_val_num);
					printf("%sden = %s;\n", id, fract_val_den);
					break;
				}
			}
			break;
		}
		case TAC_SUM: 
		case TAC_DIFF:
		{
			char * numA = getValue(entry->arg0, "num");
			char * denA = getValue(entry->arg0, "den");
			
			char * numB = getValue(entry->arg1, "num");
			char * denB = getValue(entry->arg1, "den");
			printf("/* SUM */\n");
			printf("h0 = %s * %s;\n", numA, denB);
			printf("h1 = %s * %s;\n", numB, denA);
			printf("h2 = h0 %s h1;\n", entry->op==TAC_SUM?"+":"-");
			printf("h3 = %s * %s;\n", denA, denB);
			printf("h4 = MCD(h2, h3);\n");
			printf("t%pnum = h2 / h5;\n", entry);
			printf("t%pden = h3 / h5;\n", entry);
			free(numA);
			free(numB);
			free(denA);
			free(denB);
			break;
		}
		case TAC_MULT: 
		case TAC_DIV:
		{
			char * numA = getValue(entry->arg0, "num");
			char * denA = getValue(entry->arg0, "den");
			
			char * numB = getValue(entry->arg1, "num");
			char * denB = getValue(entry->arg1, "den");
			if(entry->op == TAC_DIV){ //Take the inverse for division
				char * tmp = numB;
				numB = denB;
				denB = tmp;
			}
			
			printf("/* START %s */\n", entry->op == TAC_MULT?"MULT":"DIV");
			printf("h0 = %s * %s;\n", numA, numB);
			printf("h1 = %s * %s;\n", denA, denB);
			printf("h2 = MCD(h0, h1);\n");
			printf("t%pnum = h0 / h2;\n", entry);
			printf("t%pden = h1 / h3;\n", entry);
			printf("/* END %s */\n", entry->op == TAC_MULT?"MULT":"DIV");
			free(numA);
			free(numB);
			free(denA);
			free(denB);
			break;
		
		}
		case TAC_PRINT:
		{
			char * id = entry->arg0->address->id;
			switch(getType(id)){
				case BOOL_T: printf("printf(\"%cd\", %s)\n", 37, id); break;
				case FRACT_T: printf("printf(\"[%%d | %%d]\", %snum, %sden)\n", id, id); break;
			}
			break;
		}
		case TAC_NOT: 
		{
			char * boolean_value = getBooleanValue(entry->arg0);
			printf("t%p = !%s;\n", entry, boolean_value);
			free(boolean_value);
			break;
		}
		case TAC_AND:
		case TAC_OR:
		{
			char * bool1 = getBooleanValue(entry->arg0);
			char * bool2 = getBooleanValue(entry->arg1);
			printf("t%p = %s %s %s;\n", entry, bool1,  get_operator(entry->op), bool2);
			break;
		}
		case TAC_EQ:
		case TAC_NEQ:
		{
			char * id = entry->arg0->address->id;

			switch(getType(id)){
				case BOOL_T:
				{
					char * bool1 = getBooleanValue(entry->arg0);
					char * bool2 = getBooleanValue(entry->arg1);
					printf("t%p = %s %s %s", entry, bool1, get_operator(entry->op), bool2);
					break;
				}
				case FRACT_T:
				{
					char * numA = getValue(entry->arg0, "num");
					char * denA = getValue(entry->arg0, "den");
					char * numB = getValue(entry->arg1, "num");
					char * denB = getValue(entry->arg1, "den");
					printf("h0 = %s %s %s;\n", numA, get_operator(entry->op), numB);
					printf("h1 = %s %s %s;\n", denA, get_operator(entry->op), denB);
					printf("t%p = h0 && h1;\n", entry);
					break;
				}
			}
			break;
		}
		case TAC_LT:
		case TAC_GEQ:
		case TAC_LEQ:
		case TAC_GT:
		{
			char * numA = getValue(entry->arg0, "num");
			char * denA = getValue(entry->arg0, "den");
			char * numB = getValue(entry->arg1, "num");
			char * denB = getValue(entry->arg1, "den");
			printf("h0 = %s * %s;\n", numA, denB);
			printf("h1 = %s * %s;\n", denA, numB);
			printf("t%p = h0 %s h1;\n", entry, get_operator(entry->op));
			break;
		}
		case TAC_LABEL:
		{
			printf("L%p:\n", entry);
			break;
		}
		case TAC_COND:
		{
			char * boolean_value = getBooleanValue(entry->arg0);
			printf("t%p = %s\n", entry, boolean_value);
			free(boolean_value);
			break;
		}
		case TAC_GOTO:
		{
			if(entry->arg1 != NULL){ //Codnitioned goto
				printf("if (t%p) goto L%p;\n", entry->arg0->instruction, entry->arg1->instruction);
			} else {
				printf("goto L%p;\n", entry->arg0->instruction);
			}
			break;
		}
		default:printf("Not yet implemented\n");
	} 

	
	
}

static
char * get_operator(tac_op operator){
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
		case TAC_EQ: return "=="; break;
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
		default: yyerror("TAC_Printer - operator not recognized"); break;
	}
	return "";
}

static 
char * getBooleanValue(tac_value * tvalue){
	if(tvalue->address != NULL){
		char * buffer = malloc(sizeof(char) * (strlen(tvalue->address->id) + 1));
		sprintf(buffer, "%s", tvalue->address->id);
		return buffer;
	} else if(tvalue->instruction != NULL){
		char * buffer = malloc(sizeof(char) * 128);
		sprintf(buffer, "t%p", tvalue->instruction);
		return buffer;
	}
	 else {
		char * buffer = malloc(sizeof(char) * 2);
		sprintf(buffer, "%d", tvalue->boolean);
		return buffer;
	}

}

static
char * getValue(tac_value * tvalue, char * num_or_den) {
	if(tvalue->address != NULL){
		char * buffer = malloc(sizeof(char) * (strlen(tvalue->address->id) + strlen(num_or_den) + 1));
		sprintf(buffer, "%s%s", tvalue->address->id, num_or_den);
		return buffer;
	}
	else if(tvalue->fract != NULL){
		char * buffer = malloc(sizeof(char) * 128);
		if(strcmp(num_or_den, "num") == 0)
			sprintf(buffer, "%d", tvalue->fract->num);
		else
			sprintf(buffer, "%d", tvalue->fract->den);
		return buffer;
	} else if(tvalue->instruction != NULL){
		char * buffer = malloc(sizeof(char) * 128);
		sprintf(buffer, "t%p%s", tvalue->instruction, num_or_den);
		return buffer;
	} else {
		yyerror("Error in fetching numerator %s %d \n", __FILE__, __LINE__);
		return "";
	}
}

/*static
void print_operator(tac_op operator){
	switch(operator){
		case TAC_SUM: printf("+"); break;
		case TAC_DIFF: printf("-"); break;
		case TAC_MULT: printf("*"); break;
		case TAC_DIV: printf("/"); break;
		case TAC_NOT: printf("!"); break;
		case TAC_AND: printf ("&&"); break;
		case TAC_OR: printf ("||"); break;
		case TAC_LT: printf ("<"); break;
		case TAC_GT: printf (">"); break;
		case TAC_LEQ: printf ("<="); break;
		case TAC_GEQ: printf (">="); break;
		case TAC_NEQ: printf( "!="); break;
		case TAC_PRINT: printf("printf"); break;
		case TAC_ASSIGNMENT: printf("="); break;
		case TAC_GOTO: printf("goto"); break;
		case TAC_LABEL: printf("label"); break;
		// the others are all unrecognized operators 
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

}
*/
