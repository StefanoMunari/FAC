#include "tac.h"
#include "tac_ast_node.h"
#include "stack.h"
#include "tac_list.h"
#include <stdio.h>

static
stack_t stack;
static
void tac(seq_node * node, tac_node ** successor, tac_node * current);
static
void init_tac(tac_node *);


void generate_tac(seq_node * input, tac_node ** result){
	tac(input, result, NULL);
}

static
void tac(seq_node * node, tac_node ** successor, tac_node * current){
	printf("---TAC---\n");
	if(!node)
		init_tac(current);
	if(node->left)
		tac(node->left, successor, current);
	if(node->right)
		tac_ast_node(node->right, successor, current, &stack);
}

static
void init_tac(tac_node * current){
	current=NULL;
	init(&stack);
}