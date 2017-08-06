#include "tac_printer.h"
#include "../tac_list.h"
#include "../../utils/get_option.h"
#include "../../symbol_table/symbol_table.h"

static
void print_tac(tac_list *, char * out_dir, char * out_file);

const struct _tprinter_vtable C[] = { { print_tac } };

static
void _dump_symbol_table(FILE *);

static
void _print_h_vars(FILE *);

static
void _print_tac_entry(FILE *, tac_node *, FILE *);

static
char * _get_operator(tac_op operator);

static
char * _get_value(tac_value *, char *);

static
char * _get_boolean_value(tac_value *);

static
bool h_flag = false;

void print_tac(tac_list * tlist, char * out_dir, char * out_file){
	if(out_dir == NULL)
		yyerror("tac_printer_c::print_tac:Invalid output directory");
	if(out_file == NULL)
		yyerror("tac_printer_c::print_tac:Invalid output file");

	char buffer0[sizeof(char) * (strlen(out_dir) + strlen(out_file))];
	sprintf(buffer0, "%s/%s", out_dir, out_file);
	char * main_name = strdup(buffer0);
	char buffer1[sizeof(char) * (strlen(out_dir) + strlen("fvariables.h"))];
	sprintf(buffer1, "%s/%s",  out_dir, "fvariables.h");
	char * header_name = strdup(buffer1);
	printf("%s\n", main_name);

	FILE * c_main = fopen(main_name, "w");
	if(c_main == NULL)
		yyerror("tac_printer_c::print_tac:Could not open %s", main_name);
	FILE * c_header = fopen(header_name, "w");
	if(c_header == NULL)
		yyerror("tac_printer_c::print_tac:Could not open %s", header_name);

	_dump_symbol_table(c_header);

	fprintf(c_main, "#include \"fvariables.h\"\n");
	fprintf(c_main, "#include <stdio.h>\n");
	fprintf(c_main, "int MCD(int u, int v) {\n");
	fprintf(c_main, "\tif(u < 0)u=-u;\n\tif(v < 0)v=-v;\n"); 
	fprintf(c_main, "\treturn (v != 0)?MCD(v, u %c v):u;\n}\n", 37);

	fprintf(c_main, "int main(void){\n");

	int i=0;
	tac_node * iterator = tlist->first;
	while(iterator != NULL){
		_print_tac_entry(c_main, iterator, c_header);
		++i;
		iterator = iterator->next;
	}
	fprintf(c_main, "return 0;\n");
	fprintf(c_main, "}\n");

	/* Close the files and check if error happens */
	if(fclose(c_main) != 0)
		yyerror("tac_printer_c::print_tac:Could not close %s", main_name);
	if(fclose(c_header) != 0)
		yyerror("tac_printer_c::print_tac:Could not close %s", header_name);
	free(main_name);
	free(header_name);

}

void _dump_symbol_table(FILE * c_header){
	symbol_table_entry * iterator;
	symbol_table_entry * table = get_table();

	for(iterator = table; iterator != NULL; iterator = iterator->hh.next){
		switch(iterator->type) {
			case BOOL_T:
				fprintf(c_header, "int %s;\n", iterator->id);
				break;
			case FRACT_T:
				fprintf(c_header, "int %snum;\n", iterator->id);
				fprintf(c_header, "int %sden;\n", iterator->id);
				break;
			default:
				yyerror("tac_printer_c::_dump_symbol_table:\
						wrong symbol table type"
				);
				break;
		}
    }
}


void _print_h_vars(FILE * c_header){
	if(h_flag)
		return;
	int i=0;
	for(;i < 5; ++i)
		fprintf(c_header, "int h%d;\n", i);
	h_flag = true;
}

void _print_tac_entry(FILE * c_main, tac_node * node, FILE * c_header){
	tac_entry * entry = node->value;
	if(entry == NULL)
		return;
	switch(entry->op){
		case TAC_ASSIGNMENT:
		{
			char * id = entry->arg0->address->id;
			switch(get_type(id)){
				case BOOL_T:
				{
					char * boolean_value = _get_boolean_value(entry->arg1);
					fprintf(c_main, "%s = %s;\n", id, boolean_value);

					free(boolean_value);
					break;
				}
				case FRACT_T:
				{
					char * fract_val_num = _get_value(entry->arg1, "num");
					char * fract_val_den = _get_value(entry->arg1, "den");
					fprintf(c_main, "%snum = %s;\n", id, fract_val_num);
					fprintf(c_main, "%sden = %s;\n", id, fract_val_den);

					free(fract_val_num);
					free(fract_val_den);
					break;
				}
			}
			break;
		}
		case TAC_PLUS:
		case TAC_MINUS:
		{
			char * numA = _get_value(entry->arg0, "num");
			char * denA = _get_value(entry->arg0, "den");
			fprintf(c_main, "t%pnum = %s %s;\n",
						entry, entry->op==TAC_SUM?"+":"-", numA
			);
			fprintf(c_main, "t%pden = %s;\n",
						entry, denA
			);

			fprintf(c_header, "int t%pnum;\n", entry);
			fprintf(c_header, "int t%pden;\n", entry);

			free(numA);
			free(denA);
			break;
		}
		case TAC_SUM:
		case TAC_DIFF:
		{

			char * numA = _get_value(entry->arg0, "num");
			char * denA = _get_value(entry->arg0, "den");
			char * numB = _get_value(entry->arg1, "num");
			char * denB = _get_value(entry->arg1, "den");

			fprintf(c_main, "/* SUM */\n");
			fprintf(c_main, "h0 = %s * %s;\n", numA, denB);
			fprintf(c_main, "h1 = %s * %s;\n", numB, denA);
			fprintf(c_main, "h2 = h0 %s h1;\n", entry->op==TAC_SUM?"+":"-");
			fprintf(c_main, "h3 = %s * %s;\n", denA, denB);
			fprintf(c_main, "h4 = MCD(h2, h3);\n");
			fprintf(c_main, "t%pnum = h2 / h4;\n", entry);
			fprintf(c_main, "t%pden = h3 / h4;\n", entry);

			fprintf(c_header, "int t%pnum;\n", entry);
			fprintf(c_header, "int t%pden;\n", entry);
			_print_h_vars(c_header);


			free(numA);
			free(numB);
			free(denA);
			free(denB);

			break;
		}
		case TAC_MULT:
		case TAC_DIV:
		{
			char * numA = _get_value(entry->arg0, "num");
			char * denA = _get_value(entry->arg0, "den");

			char * numB = _get_value(entry->arg1, "num");
			char * denB = _get_value(entry->arg1, "den");
			if(entry->op == TAC_DIV){ //Take the inverse for division
				char * tmp = numB;
				numB = denB;
				denB = tmp;
			}

			fprintf(c_main, "/* START %s */\n", entry->op == TAC_MULT?"MULT":"DIV");
			fprintf(c_main, "h0 = %s * %s;\n", numA, numB);
			fprintf(c_main, "h1 = %s * %s;\n", denA, denB);
			fprintf(c_main, "h2 = MCD(h0, h1);\n");
			fprintf(c_main, "t%pnum = h0 / h2;\n", entry);
			fprintf(c_main, "t%pden = h1 / h2;\n", entry);
			fprintf(c_main, "/* END %s */\n", entry->op == TAC_MULT?"MULT":"DIV");

			fprintf(c_header, "int t%pnum;\n", entry);
			fprintf(c_header, "int t%pden;\n", entry);
			_print_h_vars(c_header);

			free(numA);
			free(numB);
			free(denA);
			free(denB);
			break;

		}
		case TAC_PRINT:
		{
			char * id = entry->arg0->address->id;
			switch(get_type(id)){
				case BOOL_T:
					fprintf(c_main, "printf(\"%cd\\n\", %s);\n", 37, id);
					break;
				case FRACT_T:
					fprintf(c_main, "printf(\"[%%d|%%d]\\n\", %snum, %sden);\n",
								id, id
					);
					break;
			}
			break;
		}
		case TAC_NOT:
		{
			char * boolean_value = _get_boolean_value(entry->arg0);
			fprintf(c_main, "t%p = !%s;\n", entry, boolean_value);

			fprintf(c_header, "int t%p;\n", entry);

			free(boolean_value);
			break;
		}
		case TAC_AND:
		case TAC_OR:
		{
			char * bool1 = _get_boolean_value(entry->arg0);
			char * bool2 = _get_boolean_value(entry->arg1);
			fprintf(c_main,
						"t%p = %s %s %s;\n",
						entry, bool1, _get_operator(entry->op), bool2
			);

			fprintf(c_header, "int t%p;\n", entry);

			free(bool1);
			free(bool2);
			break;
		}
		case TAC_XOR:
		case TAC_IFF:
		{
			char * bool1 = _get_boolean_value(entry->arg0);
			char * bool2 = _get_boolean_value(entry->arg1);
			fprintf(c_main, "t%p = %s %s %s; \n",
						entry, bool1, _get_operator(entry->op), bool2
			);
			fprintf(c_header, "int t%p;\n", entry);

			free(bool1);
			free(bool2);
			break;
		}
		case TAC_EQ:
		case TAC_NEQ:
		{

			char * numA = _get_value(entry->arg0, "num");
			char * denA = _get_value(entry->arg0, "den");
			char * numB = _get_value(entry->arg1, "num");
			char * denB = _get_value(entry->arg1, "den");
			fprintf(c_main, "h0 = %s %s %s;\n", numA, _get_operator(entry->op), numB);
			fprintf(c_main, "h1 = %s %s %s;\n", denA, _get_operator(entry->op), denB);
			fprintf(c_main, "t%p = h0 && h1;\n", entry);

			fprintf(c_header, "int t%p;\n", entry);
			_print_h_vars(c_header);

			free(numA);
			free(denA);
			free(numB);
			free(denB);
			break;
		}
		case TAC_LT:
		case TAC_GEQ:
		case TAC_LEQ:
		case TAC_GT:
		{
			char * numA = _get_value(entry->arg0, "num");
			char * denA = _get_value(entry->arg0, "den");
			char * numB = _get_value(entry->arg1, "num");
			char * denB = _get_value(entry->arg1, "den");
			fprintf(c_main, "h0 = %s * %s;\n", numA, denB);
			fprintf(c_main, "h1 = %s * %s;\n", denA, numB);
			fprintf(c_main, "t%p = h0 %s h1;\n", entry, _get_operator(entry->op));

			fprintf(c_header, "int t%p;\n", entry);
			_print_h_vars(c_header);

			free(numA);
			free(denA);
			free(numB);
			free(denB);
			break;
		}
		case TAC_LABEL:
		{
			fprintf(c_main, "L%p:\n", entry);
			break;
		}
		case TAC_GOTO:
		{
			if(entry->arg1 != NULL){ //Conditional goto
				fprintf(c_main, "if(t%p) goto L%p;\n", entry->arg0->instruction,
				entry->arg1->instruction);
			} else {
				fprintf(c_main, "goto L%p;\n", entry->arg0->instruction);
			}
			break;
		}
		default:
			yyerror("tac_printer_c:_print_tac_entry:%d operator not recognized",
				entry->op);
	}
}

static
char * _get_operator(tac_op operator){
	switch(operator){
		case TAC_PLUS:
		case TAC_SUM: return "+"; break;
		case TAC_MINUS:
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
		case TAC_IFF: return "=="; break;
		case TAC_XOR: return "!="; break;
		case TAC_PRINT: return "printf"; break;
		case TAC_ASSIGNMENT: return "="; break;
		case TAC_GOTO: return "goto"; break;
		case TAC_LABEL: return "label"; break;
		/* the others are all unrecognized operators */
		default:
			yyerror("tac_pritner:_get_operator - operator not recognized");
			break;
	}
	return "";
}

static
char * _get_boolean_value(tac_value * tvalue){
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
char * _get_value(tac_value * tvalue, char * num_or_den) {
	if(tvalue == NULL){
		return NULL;
	}
	if(tvalue->address != NULL){
		char * buffer =
			malloc(sizeof(char) * (strlen(tvalue->address->id)
			+ strlen(num_or_den) + 1));
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
		yyerror("tac_printer_c:_get_value:Error in fetching numerator %s %d \n");
		return "";
	}
}
