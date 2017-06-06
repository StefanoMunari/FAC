#include "tac_list.h"

tac_list * tac_append(tac_list * tlist, tac_list * to_append){

	if(!to_append->first && !to_append->last){
		free(to_append);
		return tlist;
	}
	if(!tlist->last && !tlist->first){
		/* first node of the TAC list - nothing to connect */
		tlist->first = to_append->first;
		tlist->last = to_append->last;
		free(to_append);
		return tlist;
	}

	tlist->last->next = to_append->first;
	to_append->first->prev = tlist->last;
	tlist->last = to_append->last;

	free(to_append);

	return tlist;
}

tac_list * tac_connect(tac_list * tlist, tac_node * tnode){
	if(!tlist->last && !tlist->first){
		/* first node of the TAC list - nothing to connect */
		tlist->first = tnode;
		tlist->last = tnode;
	}
	else{
		if(tnode == NULL)
			return tlist;
		tlist->last->next=tnode;
		tnode->prev = tlist->last;
		tlist->last = tlist->last->next;
		tnode->next = NULL;
	}
	return tlist;
}
