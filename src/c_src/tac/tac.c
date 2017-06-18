#include "tac.h"
#include "tac_ast_node.h"
#include "tac_list.h"



static
tac_list * _init_tac(tac_list *);
static
void _free_tac_entry(tac_entry * entry);


tac_list * generate_tac(seq_node * node){
	tac_list * tlist = calloc(1, sizeof(tac_list));
	if(!node->left)
		_init_tac(tlist);
	else {
		tac_list * help = generate_tac(node->left);
		tlist = tac_append(tlist, help);
	}
	if(node->right){
		tac_list * help = tac_ast_node(node->right);
		tlist = tac_append(tlist, help);
	}
	return tlist;
}


static
tac_list * _init_tac(tac_list * tlist){
	tlist->first=NULL;
	tlist->last=NULL;
	return tlist;
}

void free_tac(tac_list * tlist){
	if(!tlist)
		return;
	if(!tlist->first || !tlist->last){
		free(tlist);
		return;
	}
	tac_node * iterator = tlist->last;
	while(iterator){
		_free_tac_entry(iterator->value);
		tac_node * old = iterator;
		iterator = iterator->prev;
		free(old);
	}
	free(tlist);
}

static
void _free_tac_entry(tac_entry * entry){
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
