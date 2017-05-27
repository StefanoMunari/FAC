#include "tac_printer.h"
#include <string.h>

void tdynamic_dispatch(struct tprinter* this, tac_list* parameter){
	this->_vtable->print_tac(parameter);
}