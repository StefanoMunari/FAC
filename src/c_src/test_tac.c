#include "test_tac.h"

void test_tac(tac_node ** head){
	tac_node * iterator=(* head);
	int i=0;
	while(iterator){
		printf("ITERATOR %p\n", iterator);
		iterator=iterator->next;
		++i;
	}
}