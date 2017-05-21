#include "tac_ast_node.h"
#include "factype_ast.h"
#include "factype_tac.h"
#include "tac.h"
#include <limits.h>
#include <stdio.h>

extern void yyerror(char *);

static
tac_node* _tac_node();
static
tac_list* _tac_connect(tac_list *, tac_node *);
static
tac_list* _tac_fract(ast_node *);
static
tac_list* _tac_bool(ast_node *);
static
tac_list* _tac_id(ast_node *);
static
tac_list* _tac_print(tac_list *, ast_node *);

/** Append the list toAppend to the given list
 * @param list the list that encodes the first part
 * @param toAppend a list to append
 * @return a pointer to the head of the new list. It corresponds to the
 * head of list
 */
tac_list * append(tac_list * list, tac_list * toAppend){
	toAppend->first->prev = list->last;
	list->last->next = toAppend->first;
	toAppend->first = list->first;
	return list;
}

/**
* @brief builds the 3AC list of triples from the last to the first node by
*	traversing the AST bottom-up
* @note opt_t and tac_op have the same value because the enums follow
* 		the same declaration order
*
*/
tac_list * tac_ast_node(ast_node * node, tac_list * tlist, stack_t * stack){
	if(node == NULL || node->data == NULL){
		yyerror("TAC - malformed AST, null node found");
		return tlist;
	}
	switch(node->data->token){
		/* Internal nodes */
		case AST_AOP1:
		case AST_BOP1:
		{
			tac_node* tnode=_tac_node();
			/* set up tnode */
			tnode->value->op = node->data->op;
			tlist=_tac_connect(tlist, tnode);
			/* compute child node */
			tac_list* left=tac_ast_node(node->ast_children[0], tlist, stack);
			/* the child node is a subtree */
			if(tlist->last != left->last){
				/* set up 3AC */
				tlist->last->value->arg0 = calloc(1, sizeof(tac_value));
				tlist->last->value->arg0->instruction =  left->last->value;
				tlist->last->value->arg1=NULL;
				/* connect the list of triples */
				tlist = append(left, tlist);
			}
			return tlist;
		}
		case AST_AOP2:
		case AST_BOP2:
		case AST_RELOP:
		{
			/* compute child node */
			tac_list* right=tac_ast_node(node->ast_children[1], tlist, stack);
			tac_list* left=tac_ast_node(node->ast_children[0], tlist, stack);
			tac_node* tnode=NULL;
			/* setup 3AC */
			if(right->last->value->op != -1 || left->last->value->op != -1){
				tnode=_tac_node();
				if(left->last->value->op != -1){
					printf("AOP2-L-subtree\n");
					tnode->value->arg0 = calloc(1, sizeof(tac_value));
					tnode->value->arg0->instruction = left->last->value;
				}
				if(right->last->value->op != -1){
					printf("AOP2-R-subtree\n");
					tnode->value->arg1 = calloc(1, sizeof(tac_value));
					tnode->value->arg1->instruction = right->last->value;
				}
			}
			else{
				printf("AOP2-leaf\n");
				tnode=_tac_node();
				tnode->value->arg0=left->last->value->arg1;
				tnode->value->arg1=right->last->value->arg1;
			}
			/* set up tnode */
			tnode->value->op = node->data->op;
			tlist=_tac_connect(tlist, tnode);
			return tlist;
		}
		case AST_ASSIGNMENT:
		{
			/* compute child node */
			tac_list* right=tac_ast_node(node->ast_children[1], tlist, stack);
			tac_node* tnode=NULL;
			/* setup 3AC */
			if(right->last->value->op != -1){
				printf("ASS-subtree\n");
				tnode=_tac_node();
				tnode->value->arg1 = calloc(1, sizeof(tac_value));
				tnode->value->arg1->instruction =  right->last->value;
			}
			else{
				printf("ASS-leaf\n");
				tnode=right->last;
			}
			/* set up tnode */
			tnode->value->op = TAC_ASSIGNMENT;
			/* left side of assignment */
			tnode->value->arg0 = calloc(1, sizeof(tac_value));
			tnode->value->arg0->address = lookupID(node->ast_children[0]->data->value);
			/* connect list of triples */
			tlist=_tac_connect(tlist, tnode);
			return tlist;
		}
		/* Leaves */
		case AST_FRACT:{
			return _tac_fract(node);
		}
		case AST_BOOL:{
			return _tac_bool(node);
		}
		case AST_ID:
		{
			return _tac_id(node);
		}
		case AST_PRINT:
		{
			tac_node * tnode=_tac_node();
			return _tac_print(_tac_connect(tlist, tnode), node);
		}
		case AST_DECLARATION:{
			return tlist;
		}
		default:
			yyerror("TAC - token not recognized");
			exit(EXIT_FAILURE);
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
	node->value->op = -1;
	node->value->arg0 = NULL;
	node->value->arg1 = NULL;
	node->prev=NULL;
	node->next=NULL;
	return node;
}

static
tac_list * _tac_connect(tac_list * tlist, tac_node * tnode){
	if(!tlist->last && !tlist->first){
		tlist->first=tnode;
		tlist->last=tnode;
		/* first node of the TAC list representation - nothing to connect */
	}
	else{
		tlist->last->next=tnode;
		tnode->prev=tlist->last;
		tlist->last=tlist->last->next;
	}
	return tlist;
}

static
tac_list * _tac_fract(ast_node * node){
	tac_list * tlist=malloc(sizeof(tac_list));
	tlist->last=_tac_node();
	/* convention - set always arg1 */
	tlist->last->value->arg1 = calloc(1, sizeof(tac_value));
	tlist->last->value->arg1->fract = (fract_t*) node->data->value;
	return tlist;
}

static
tac_list * _tac_bool(ast_node * node){
	tac_list * tlist=malloc(sizeof(tac_list));
	tlist->last=_tac_node();
	/* convention - set always arg1 */
	tlist->last->value->arg1 = calloc(1, sizeof(tac_value));
	tlist->last->value->arg1->boolean = node->data->value;
	return tlist;
}

static
tac_list * _tac_id(ast_node * node){
	tac_list * tlist=malloc(sizeof(tac_list));
	tlist->last=_tac_node();
	printf("tac-id\n");
	/* convention - set always arg1 */
	tlist->last->value->arg1 = calloc(1, sizeof(tac_value));
	tlist->last->value->arg1->address = (symbol_table_entry *) lookupID(node->data->value);
	return tlist;
}

static
tac_list * _tac_print(tac_list * tlist, ast_node * node){
	tlist->last->value->op = TAC_PRINT;
	tlist->last->value->arg0 = calloc(1, sizeof(tac_value));
	tlist->last->value->arg0->address = lookupID(node->ast_children[0]->data->value);
	return tlist;
}
