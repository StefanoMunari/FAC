#ifndef __TAC_PRINTER_C_H__
#define __TAC_PRINTER_C_H__
#include "tac_printer.h"
#include "../tac_list.h"

void print_tac_c(tac_list *);
struct _tprinter_vtable C[] = { { print_tac_c } };
#endif /*__TAC_PRINTER_C_H__*/
