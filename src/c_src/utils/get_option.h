/**
 * @file
 * @author mirko.bez@studenti.unipd.it
 * @author stefano.munari.1@studenti.unipd.it
 * @brief files containing the utilities to fetch the options from the command
 * line.
 */
#ifndef __GET_OPTION_H__
#define __GET_OPTION_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libgen.h>
#include <string.h>
#include <getopt.h>
#include "../tac/printer/tac_printer.h"

/** @brief structure that encodes fetched options */
typedef struct {
	/** true if the user want to print the ast, false otherwise */
	bool print_ast;
	/** name of the input file */
	char * input_file;
	/** name of the output dir */
	char * output_dir;
	/** name of the output file */
	char * output_main_file;
	/** printer used to print the three address code */
	tprinter printer;
} option_flag;


/**
 * get the options from the command line
 * @param argc the argument counter
 * @param argv the argument vector
 * @return an option_flag object containing the user-defined options.
 */
option_flag get_option(int argc,  char * argv[]);

/**
 * free the fields of the given option field
 * @param options the option field to free
 */
void free_option_flag(option_flag options);

#endif /* __GET_OPTION_H__ */
