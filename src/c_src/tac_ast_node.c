#include "tac_ast_node.h"
#include "factype_ast.h"
#include "factype_tac.h"
#include "tac.h"
#include <stdio.h>
#include <limits.h>

extern void yyerror(char *);

static
tac_node* _tac_node();
static
tac_list* _tac_fract(tac_list *, ast_node *);
static
tac_list* _tac_bool(tac_list *, ast_node *);
static
tac_list* _tac_id(tac_list *, ast_node *);
static
tac_list* _tac_print(tac_list *, ast_node *);


/**
* @brief builds the 3AC list of triples from the last to the first node by
*	traversing the AST bottom-up
* @note opt_t and tac_op have the same value because the enums follow
* 		the same declaration order
*
*/
tac_list * tac_ast_node(ast_node * node, tac_list * tlist, stack_t * stack){
	printf("----TAC_AST-NODE-----\n");
	if(node == NULL || node->data == NULL){
		yyerror("TAC - malformed AST, null node found");
		return tlist;
	}
	switch(node->data->token){
		/* Internal nodes */
		case AST_AOP1:
		case AST_BOP1:
		{
			printf("AOP1.BOP1\n");
			tac_node* tnode=_tac_node();
			/* set up tnode */
			tnode->value->op = node->data->op;
			if(tlist->last == NULL)
				tlist->last=tnode;
			else
				tlist->last->next=tnode;
			tlist->last->next=tnode;
			tnode->prev=tlist->last;
			tlist->last=tlist->last->next;
			/* compute child node */
			tac_list* left=tac_ast_node(node->ast_children[0], tlist, stack);
			/* the child node is a subtree */
			if(tlist->last != left->last){
				/* set up 3AC */
				tlist->last->value->arg0 = malloc(sizeof(tac_value));
				tlist->last->value->arg0->instruction =  left->last->value;
				tlist->last->value->arg1=NULL;
				/* connect the list of triples */
				tlist->first->prev=left->last;
				left->last->next=tlist->first;
				tlist->first=left->first;
			}
			printf("----------return AOP1.BOP1---------------------------\n");
			printf("-------------------------------------\n");
			return tlist;
		}
		case AST_AOP2:
		case AST_BOP2:
		case AST_RELOP:
		{
			printf("AOP2.BOP2.RELOP\n");
			tac_node* tnode=_tac_node();
			/* set up tnode */
			tnode->value->op = node->data->op;
			if(tlist->last == NULL)
				tlist->last=tnode;
			else
				tlist->last->next=tnode;
			tlist->last->next=tnode;
			tnode->prev=tlist->last;
			tlist->last=tlist->last->next;
			/* compute child node */
			tac_list* left=tac_ast_node(node->ast_children[0], tlist, stack);
			tac_list* right=tac_ast_node(node->ast_children[1], tlist, stack);
			/* setup 3AC */
			if(tlist->last != left->last){
				tlist->last->value->arg0 = malloc(sizeof(tac_value));
				tlist->last->value->arg0->instruction =  left->last->value;
			}
			if(tlist->last != right->last){
				tlist->last->value->arg1 = malloc(sizeof(tac_value));
				tlist->last->value->arg1->instruction =  right->last->value;
			}
			/* connect the list of triples */
			if(tlist->last != left->last){
				tlist->first->prev=left->last;
				left->last->next=tlist->first;
				tlist->first=left->first;
			}
			if(tlist->last != right->last){
				tlist->first->prev=right->last;
				right->last->next=tlist->first;
				tlist->first=right->first;
			}
			printf("----------return AOP1.BOP1---------------------------\n");
			printf("-------------------------------------\n");
			return tlist;
		}
		case AST_ASSIGNMENT:
		{
			printf("ASS\n");
			tac_node* tnode=_tac_node();
			/* set up tnode */
			tnode->value->op = TAC_ASSIGNMENT;
			if(tlist->last == NULL)
				tlist->last=tnode;
			else{
				printf("-ASDASd-%p\n", tlist->last);
				tlist->last->next=tnode;
			}
			tnode->prev=tlist->last;
			tlist->last=tlist->last->next;
			/* left side of assignment */
			tnode->value->arg0 = malloc(sizeof(tac_value));
			tnode->value->arg0->address = lookupID(node->ast_children[0]->data->value);
			/* compute child node */
			tac_list* right=tac_ast_node(node->ast_children[1], tlist, stack);
			/* setup 3AC */
			if(tlist->last != right->last){
				tlist->last->value->arg0 = malloc(sizeof(tac_value));
				tlist->last->value->arg0->instruction =  right->last->value;
			}
			/* connect list of triples */
			if(tlist->last != right->last){
				tlist->first->prev=right->last;
				right->last->next=tlist->first;
				tlist->first=right->first;
			}
			printf("-----------------return ASS--------------------\n");
			printf("-------------------------------------\n");
			return tlist;
		}
		/* Leaves */
		case AST_FRACT:{
			return _tac_fract(tlist, node);
		}
		case AST_BOOL:{
			return _tac_bool(tlist, node);
		}
		case AST_ID:
		{
			return _tac_id(tlist, node);
		}
		case AST_PRINT:
		{
			printf("PRINT");
			tlist->last->next=_tac_node();
			tlist->last->next->prev=tlist->last;
			tlist->last=tlist->last->next;
			return _tac_print(tlist, node);
		}
		case AST_DECLARATION:{
			printf("DECL.SKIP\n");
			return tlist;
		}
		default:
			yyerror("TAC - token not recognized");
	}
}

/********************************************
			PRIVATE FUNCTIONS
*********************************************/
static
tac_node* _tac_node(){
	tac_node* node;
	node = malloc(sizeof(tac_node));
	node->value = malloc(sizeof(tac_entry));
	node->value->arg0 = NULL;
	node->value->arg1 = NULL;
	node->prev=NULL;
	node->next=NULL;
	return node;
}

static
tac_list * _tac_fract(tac_list * tlist, ast_node * node){
	printf("FRACT\n");
	if(tlist->last->value->arg0){
		printf("FRACT-arg1\n");
		tlist->last->value->arg1 = malloc(sizeof(tac_value));
		tlist->last->value->arg1->fract = (fract_t*) node->data->value;
		return tlist;
	}
	printf("FRACT-arg0\n");
	tlist->last->value->arg0 = malloc(sizeof(tac_value));
	tlist->last->value->arg0->fract = (fract_t*) node->data->value;
	return tlist;
}

static
tac_list * _tac_bool(tac_list * tlist, ast_node * node){
	printf("BOOL\n");
	if(tlist->last->value->arg0){
		tlist->last->value->arg1 = malloc(sizeof(tac_value));
		tlist->last->value->arg1->boolean = node->data->value;
		return tlist;
	}
	tlist->last->value->arg0 = malloc(sizeof(tac_value));
	tlist->last->value->arg0->boolean = node->data->value;
	return tlist;
}

static
tac_list * _tac_id(tac_list * tlist, ast_node * node){
	printf("ID\n");
	if(tlist->last->value->arg0){
		printf("ID-arg1\n");
		tlist->last->value->arg1 = malloc(sizeof(tac_value));
		tlist->last->value->arg1->address = lookupID(node->data->value);
		return tlist;
	}
	printf("ID-arg0\n");
	tlist->last->value->arg0 = malloc(sizeof(tac_value));
	tlist->last->value->arg0->address = lookupID(node->data->value);
	return tlist;
}

static
tac_list * _tac_print(tac_list * tlist, ast_node * node){
	tlist->last->value->op = TAC_PRINT;
	tlist->last->value->arg0 = malloc(sizeof(tac_value));
	tlist->last->value->arg0->address = lookupID(node->ast_children[0]->data->value);
	return tlist;
}
