#include "tac_list.h"

tac_list * tac_append(tac_list * tlist, tac_list * to_append){

	if(!to_append->first && !to_append->last)
		return tlist;
	if(!tlist->last && !tlist->first){
		/* first node of the TAC list - nothing to connect */
		tlist->first=to_append->first;
		tlist->last=to_append->last;
		return tlist;
	}
	tlist->last->next=to_append->first;
	to_append->first->prev=tlist->last;
	tlist->last=to_append->last;
	to_append->first=tlist->first;
	return tlist;
}

tac_list * tac_connect(tac_list * tlist, tac_node * tnode){
	if(!tlist->last && !tlist->first){
		/* first node of the TAC list - nothing to connect */
		tlist->first=tnode;
		tlist->last=tnode;
	}
	else{
		if(tnode == NULL)
			return tlist;
		printf("CONNECT : %p -> %p\n", tlist->last->value, tnode->value);
		tlist->last->next=tnode;
		tnode->prev=tlist->last;
		tlist->last=tlist->last->next;
	}
	return tlist;
}
