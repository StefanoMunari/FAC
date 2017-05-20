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
void free_tac_value(tac_value * value);
static
void free_tac_entry(tac_entry * entry);


tac_list * generate_tac(seq_node * input){
	tac_list * output=malloc(sizeof(tac_list));
	output=tac(input, output);
	printf("output->first %p\n", output->first);
	printf("output->last %p\n", output->last);
	return output;
}

/*
	PRE= tlist is pointing to a valid record on the heap
*/
tac_list * tac(seq_node * node, tac_list * tlist){
	if(!node->left)
		return init_tac(tlist);
	else
		tlist=tac(node->left, tlist);
	if(node->right)
		return tac_ast_node(node->right, tlist, &stack);
	return tlist;
}

static
tac_list * init_tac(tac_list * tlist){
	init(&stack);
	tlist->first=NULL;
	tlist->last=NULL;
	return tlist;
}

void free_tac(tac_list * list){
	if(!list)
		return;
	tac_node * current = list->first;
	tac_node * tmp;
	while(current){
		free_tac_entry(current->value);
		tmp = current->next;
		current = tmp;
	}
	free(list->first);
	free(list->last);
	free(list);
}

static
void free_tac_value(tac_value * value){
	free(value->fract);
	free(value->address);
	free(value->instruction);
	free(value);
}


static
void free_tac_entry(tac_entry * entry){
	free_tac_value(entry->arg0);
	free_tac_value(entry->arg1);
	free(entry);
}
