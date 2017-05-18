#include "tac.h"
#include "tac_ast_node.h"
#include "stack.h"
#include "tac_list.h"
#include <stdio.h>

static
stack_t stack;
static
tac_list * init_tac();
static
void free_tac_value(tac_value * value);
static
void free_tac_entry(tac_entry * entry);

tac_list * tac(seq_node * node){
	tac_list * tlist;
	printf("---TAC---\n");
	if(node == NULL){
		printf("PRIMO BRANCH\n");
		return init_tac();
	}
	if(node->left != NULL){
				printf("SECONDO BRANCH\n");
		tlist=tac(node->left);
	}
	if(node->right != NULL){
						printf("TERZO BRANCH\n");
		return tac_ast_node(node->right, tlist, &stack);
	}
}

static
tac_list * init_tac(){
	init(&stack);
	tac_list * tlist= malloc(sizeof(tac_list));
	tlist->first=NULL;
	tlist->last=NULL;
	return tlist;
}

void free_tac(tac_node * head){
	tac_node * current = head;
	tac_node * tmp;
	while(current != NULL){
		free_tac_entry(current->value);
		tmp = current->next;
		current = tmp;
	}
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
	free(entry->arg0);
	free(entry->arg1);
	free(entry);
}