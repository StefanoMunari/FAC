#include "tac_printer.h"
#include <stdio.h>

void tdynamic_dispatch(struct tprinter* this, tac_list* parameter){
	printf("dynamix");
	this->_vtable->print_tac(parameter);
}