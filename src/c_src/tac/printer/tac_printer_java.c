#include "tac_printer.h"
#include "../tac_list.h"
#include "../../utils/get_option.h"
#include "../../symbol_table/symbol_table.h"

static
void print_tac(tac_list *, char * out_dir, char * out_file);

const struct _tprinter_vtable JAVA[] = { { print_tac } };

void print_tac(tac_list * tlist, char * out_dir, char * out_file){
	if(out_dir == NULL)
		yyerror("tac_printer_java::print_tac:Invalid output directory");
	if(out_file == NULL)
		yyerror("tac_printer_java::print_tac:Invalid output file");

	char buffer0[sizeof(char) * (strlen(out_dir) + strlen(out_file))];
	sprintf(buffer0, "%s/%s", out_dir, out_file);
	char * main_name = strdup(buffer0);
	printf("%s\n", main_name);

	FILE * java_main = fopen(main_name, "w");
	if(java_main == NULL)
		yyerror("tac_printer_java::print_tac:Could not open %s", main_name);


	fprintf(java_main,"public class Main {\n");
	fprintf(java_main,"public static void main(String[] args) {\n");
	fprintf(java_main,
		"System.out.println(\"Skeleton for Java code generator\");\n");
	fprintf(java_main,"}\n");
	fprintf(java_main,"}\n");

	/* Close the files and check if error happens */
	if(fclose(java_main) != 0)
		yyerror("tac_printer_java::print_tac:Could not close %s", main_name);
	free(main_name);
}
