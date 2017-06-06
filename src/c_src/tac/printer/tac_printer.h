/**
 * @brief definition of an API for printing the three adress code.
 * @file
 * @author mirko.bez@studenti.unipd.it
 * @author stefano.munari.1@studenti.unipd.it
 */

#ifndef __TAC_PRINTER_H__
#define __TAC_PRINTER_H__
#include "../tac_list.h"

struct option_flags;

typedef struct _tprinter_vtable {
   void (*print_tac)(tac_list *, char *, char *);
} _tprinter_vtable;

typedef struct tprinter {
    const struct _tprinter_vtable* _vtable;
} tprinter;

static inline
void tdynamic_dispatch(struct tprinter* this, tac_list* parameter,
	 char * out_dir, char * out_file)
{
	(*this)._vtable->print_tac(parameter, out_dir, out_file);
}

extern void yyerror(const char *, ...);
extern const struct _tprinter_vtable IR[], C[], JAVA[];
#endif /*__TAC_PRINTER_H__*/
