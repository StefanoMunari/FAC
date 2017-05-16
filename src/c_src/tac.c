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


void generate_tac(seq_node * input, tac_node ** result){
	tac(input, result, NULL);
}

static
void tac(seq_node * node, tac_node ** current, tac_node * successor){
	printf("---TAC---\n");
	if(!node)
		init_tac(successor);
	if(node->left)
		tac(node->left, current, successor);
	if(node->right)
		tac_ast_node(node->right, current, successor, &stack);
}

static
void init_tac(tac_node * successor){
	successor=NULL;
	init(&stack);
}