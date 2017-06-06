#include "get_option.h"



/** Struct containing the long options */
static struct option long_options[] = {
	/* Print a help message and exit */
	{"help", no_argument, 0, 'h'},
	/* Print the abstract sytax tree */
	{"ast", no_argument, 0, 'a'},
	/* the tac printer */
	{"tac-printer", required_argument, 0, 't'},
	/* Output file */
	{"output-file", required_argument, 0, 'o'},
	{0, 0, 0, 0},
};


/**
 * print the help message
 * @param argv in order to get the name of the compiled program
 */
static
void print_help(char * argv[]){
	putchar('\n');
	printf("Usage %s <input.f> [OPT] ... \n\n", argv[0]);
	printf("-h, --help\t\t print this message and exit\n");
	printf("-o, --output\t\t set the output file for the tac printer\n");
	printf("-a, --ast\t\t print to stdout the abstract syntax tree\n");
	printf("-t, --tac-printer\t choose the tac printer (default = C)\n");
	printf("                 \t\t - IR : prints the internal representation to stdout\n");
	printf("                 \t\t - C  : prints the TAC in a main.c file");
	putchar('\n');
	putchar('\n');
}

/**
 * Function that reads command line parameter options and put the configuration in an optionFlag data stracture.
 * @param argc the command line argument counter
 * @param argv the command line arguments
 * @return an optionFlag containing the configuration of that particular instantiation of the program.
 */
option_flags get_option(int argc,  char * argv[]){
	option_flags options;
	int c;
	if(argc < 2) {
		print_help(argv);
		exit(EXIT_FAILURE);
	}
	// One can specify only the help flag without the source file
	if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
		print_help(argv);
		exit(EXIT_SUCCESS);
	// It is an option that is not help
	} else if(argv[1][0] == '-'){
		printf("%s: invalid option -- '%s'", argv[0], argv[1]);
		printf("Try '%s --help' for more information", argv[0]);
		exit(EXIT_FAILURE);
	}

	options.input_file = strdup(argv[1]);

	//Starting from index 2, because the first place is destinated to the f file
	optind = 2;
	int option_index;

	options.print_ast = false;
	options.printer = (tprinter) { C };
	options.output_dir = NULL;
	options.output_main_file = NULL;

	while((c = getopt_long (argc, argv, "ht:o:a", long_options, &option_index)) != EOF) {
		switch(c){
			case 'h': print_help(argv); exit(EXIT_SUCCESS); break;
			case 'a': options.print_ast = true; break;
			case 'o':
				options.output_dir = strdup(dirname(optarg));
				options.output_main_file = strdup(basename(optarg));
				break;
			case 't':
				if (strcmp(optarg, "C") == 0)
					options.printer = (tprinter) { C };
				else if (strcmp(optarg, "IR") == 0)
					options.printer = (tprinter) { IR };
				else{
					fprintf(stderr, "get_option::get_option: invalid printer");
					free_option_flags(options);
					exit(EXIT_FAILURE);
				}

				break;
		}
	}
	return options;
}

void free_option_flags(option_flags options) {
	if(options.input_file != NULL)
		free(options.input_file);
	if(options.output_dir != NULL)
		free(options.output_dir);
	if(options.output_main_file != NULL)
		free(options.output_main_file);
}

