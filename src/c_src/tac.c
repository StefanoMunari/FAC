#include "tac.h"
#include "tac_ast_node.h"
#include "stack.h"
#include "tac_list.h"
#include <stdio.h>

static
stack_t stack;
static
void tac(seq_node * node, tac_node ** current, tac_node * successor);
static
void init_tac(tac_node *);
static
void free_tac_value(tac_value * value);
static
void free_tac_entry(tac_entry * entry);



void generate_tac(seq_node * input, tac_node ** result){
	tac(input, result, NULL);
}

static
void tac(seq_node * node, tac_node ** current, tac_node * successor){
	printf("---TAC---\n");
	if(node == NULL)
		init_tac(successor);
	if(node->left != NULL)
		tac(node->left, current, successor);
	if(node->right != NULL)
		tac_ast_node(node->right, current, successor, &stack);
}

static
void init_tac(tac_node * successor){
	successor=NULL;
	init(&stack);
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
