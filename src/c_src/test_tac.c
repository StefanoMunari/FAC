#include "test_tac.h"
#include "test_tac.h"
#include "factype.h"
#include "tac_list.h"
#include "tac.h"
#include <stdio.h>

void test_tac(tac_node ** head){
	tac_node * iterator=(* head);
/*
	printf("FIRST-OP = %d", iterator->value->op);
	printf("FIRST-NUM = %d", ((fract_t *) iterator->value->arg0->fract)->num);
	printf("FIRST-DEN = %d", ((fract_t *) iterator->value->arg0->fract)->den);
*/
	int i=0;
	while(iterator){
		printf("\n%d)ITERATOR %p\n", i, iterator);
		iterator=iterator->next;
		++i;
	}
}
