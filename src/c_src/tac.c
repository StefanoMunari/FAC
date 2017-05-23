#include "tac.h"
#include "tac_ast_node.h"
#include "stack.h"
#include "tac_list.h"
#include <stdio.h>

static
stack_t stack;
static
tac_list * tac(seq_node *, tac_list *);
static
tac_list * init_tac(tac_list *);
static
void free_tac_entry(tac_entry * entry);


tac_list * generate_tac(seq_node * input){
	return tac(input, malloc(sizeof(tac_list)));
}

/*
	PRE= tlist is pointing to a valid record (not NULL) on the heap
	POST= tlist is a valid TAC representation of the AST given as input
	DEFINITIONS:
	- valid TAC representation:
		a post-order linearized representation of the AST where the first
		TAC block is assigned to tlist->first while the last is assigned to
		tlist->last
*/
tac_list * tac(seq_node * node, tac_list * tlist){
	if(!node->left) 
		init_tac(tlist);
	else
		tlist=tac(node->left, tlist);
	
	if(node->right)
		tlist = tac_ast_node(node->right, tlist, &stack);

	return tlist;
}

static
tac_list * init_tac(tac_list * tlist){
	init(&stack);
	tlist->first=NULL;
	tlist->last=NULL;
	return tlist;
}

void free_tac(tac_list * tlist){
	if(!tlist || !tlist->first || !tlist->last)
		return;
	tac_node * iterator = tlist->last;
	while(iterator){
		free_tac_entry(iterator->value);
		iterator = iterator->prev;
	}
	free(tlist);
}

static
void free_tac_entry(tac_entry * entry){
	if(!entry)
		return;
	free(entry->arg0);
	free(entry->arg1);
	free(entry);
	/*
	* ignore value->fract
	* it is a responsibility of the AST to free its values
	*
	* ignore value->instruction
	* since we are using a list of triples it will be deallocated by the
	* external iterator (check free_tac)
	*
	* ignore value->address
	* it is a responsibility of the symbol table to free its values
	*/
}
