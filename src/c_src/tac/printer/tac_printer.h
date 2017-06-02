#ifndef __TAC_PRINTER_H__
#define __TAC_PRINTER_H__
#include "../tac_list.h"

typedef struct _tprinter_vtable {
   void (*print_tac)(tac_list *);
} _tprinter_vtable;

typedef struct tprinter {
    const struct _tprinter_vtable* _vtable;
} tprinter;

static inline
void tdynamic_dispatch(struct tprinter* this, tac_list* parameter)
{
	this->_vtable->print_tac(parameter);
}

extern void yyerror(const char *, ...);
extern const struct _tprinter_vtable IR[], C[], JAVA[];
#endif /*__TAC_PRINTER_H__*/
