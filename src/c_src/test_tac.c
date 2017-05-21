#include "test_tac.h"
#include "test_tac.h"
#include "factype.h"
#include "tac_list.h"
#include "tac.h"
#include <stdio.h>

void test_tac(tac_list * tlist){
	tac_node * iterator=tlist->first;
	int i=0;
	while(iterator){
		printf("\n%d)ITERATOR %p\n", i, iterator->value);
		iterator=iterator->next;
		++i;
	}
}
