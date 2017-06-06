/**
 * @brief definition of an API for printing the three address code.
 * @file
 * @author mirko.bez@studenti.unipd.it
 * @author stefano.munari.1@studenti.unipd.it
 */

#ifndef __TAC_PRINTER_H__
#define __TAC_PRINTER_H__
#include "../tac_list.h"

struct option_flags;

/**
 * @brief Virtual table of the printer interface
 * @details it is a record of function pointers related to the printer interface
 */
typedef struct _tprinter_vtable {
	/** Prints the targeted code.
		It simulates a virtual function of the printer interface */
   void (*print_tac)(tac_list *, char *, char *);
} _tprinter_vtable;

/**
 * @brief A printer type. It simulates a pure interface acting as a wrapper for
 * any printer virtual table pointer.
 */
typedef struct tprinter {
	 /** A virtual table pointer */
    const struct _tprinter_vtable* _vtable;
} tprinter;

/**
 * @brief A simple dynamic dispatching method for the printer interface.
 * @details Resolves the dynamic type of the "this" pointer at runtime invoking
 * the correct (depending on the "concrete" type) print_tac implementation
 *
 * @param this - a pointer to an instance of the printer type
 * @param data - the 3AC list of indirected triples to print
 * @param out_dir - the output folder
 * @param out_file - the output file (will contain the generated representation)
 */
static inline
void tdynamic_dispatch(struct tprinter* this, tac_list* data,
	 char * out_dir, char * out_file)
{
	this->_vtable->print_tac(data, out_dir, out_file);
}

extern void yyerror(const char *, ...);
/** The available "concrete types" declared in this interface file */
extern const struct _tprinter_vtable IR[], C[], JAVA[];
#endif /*__TAC_PRINTER_H__*/
