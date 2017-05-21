#ifndef __TAC_PRINTER_JAVA_H__
#define __TAC_PRINTER_JAVA_H__
#include "tac_list.h"
#include "tac_printer.h"

void print_tac(tac_list *);
struct _tprinter_vtable JAVA[] = { { print_tac } };
#endif /*__TAC_PRINTER_JAVA_H__*/