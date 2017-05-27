#ifndef __TAC_PRINTER_H__
#define __TAC_PRINTER_H__
#include "../tac_list.h"

typedef struct _tprinter_vtable {
    void (*print_tac)(tac_list *);
} _tprinter_vtable;

typedef struct tprinter {
    struct _tprinter_vtable* _vtable;
} tprinter;

void tdynamic_dispatch(struct tprinter* this, tac_list* parameter);

extern void yyerror(const char *, ...);
extern _tprinter_vtable IR[], C[], JAVA[];
#endif /*__TAC_PRINTER_H__*/
