#include "tac.h"
#include "../types/factype_ast.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>

extern void yyerror(const char *, ...);

static
tac_node * _tac_node();

static
tac_node * _tac_print(ast_node *);
static
tac_value * _tac_leaf(ast_node * node);

static
tac_node * _tac_label();

static
tac_node * _tac_goto_unconditioned(tac_node * destination);
static
tac_node * _tac_goto_conditioned(tac_entry * condition, tac_node * destination);

/**
* @brief builds the 3AC list of triples from the last to the first node by
*	traversing the AST bottom-up
* @note opt_t and tac_op have the same value because the enums follow
* 		the same declaration order
* @note do not free the goto NULL NULL node, reuse it to keep consistency
*/
tac_list * tac_ast_node(ast_node * node){
	if(node == NULL || node->data == NULL){
		yyerror("TAC - malformed AST, null node found");
		exit(EXIT_FAILURE);
	}
	/* Create an empty tlist */
	tac_list* tlist = calloc(1, sizeof(tac_list));
	switch(node->data->token){
		/* Internal nodes */
		case AST_AOP1:
		case AST_BOP1:
		{
			tac_node* tnode=_tac_node();
			tnode->value->op = node->data->op;
			tac_value * left = _tac_leaf(node->ast_children[0]);
			if(left != NULL){ /* It is a leaf */
				tnode->value->arg0 = left;
			} else {
				tac_list * left_list = tac_ast_node(node->ast_children[0]);
				tnode->value->arg0= calloc(1, sizeof(tac_value));
				tnode->value->arg0->instruction= left_list->last->value;
				tlist = tac_append(tlist, left_list);
			}
			/* connect tnode to the current list of triples */
			return tac_connect(tlist, tnode);
		}
		case AST_AOP2:
		case AST_BOP2:
		case AST_BOP_RELOP:
		case AST_RELOP:
		{
			/* create a new node with the right operation */
			tac_node* tnode=_tac_node();
			tnode->value->op = node->data->op;

			tac_value * left = _tac_leaf(node->ast_children[0]);

			if(left != NULL){ /* It is a leaf */
				tnode->value->arg0 = left;
			} else {
				tac_list* left = tac_ast_node(node->ast_children[0]);
				tnode->value->arg0= calloc(1, sizeof(tac_value));
				tnode->value->arg0->instruction= left->last->value;
				tlist = tac_append(tlist, left);
			}

			tac_value * right = _tac_leaf(node->ast_children[1]);
			if(right != NULL){
				tnode->value->arg1 = right;
			} else {
				tac_list* right = tac_ast_node(node->ast_children[1]);
				tnode->value->arg1= calloc(1, sizeof(tac_value));
				tnode->value->arg1->instruction= right->last->value;
				tlist = tac_append(tlist, right);
			}

			return tac_connect(tlist, tnode);
		}
		case AST_ASSIGNMENT:
		{
			tac_node* tnode=_tac_node();
			tnode->value->op = TAC_ASSIGNMENT;
			tnode->value->arg0 = calloc(1, sizeof(tac_value));
			tnode->value->arg0->address = (symbol_table_entry*)
				lookupID(node->ast_children[0]->data->value);

			/* Compute the value of the rhs */
			tac_value * leaf = _tac_leaf(node->ast_children[1]);

			if(leaf != NULL){ /* It is leaf */
				tnode->value->arg1 = leaf;
			} else { /* It is a whole list of instructions */
				tac_list* right = tac_ast_node(node->ast_children[1]);
				tnode->value->arg1 = calloc(1, sizeof(tac_value));
				tnode->value->arg1->instruction =  right->last->value;
				tlist = tac_append(tlist, right);
			}
			/* connect tnode to the current list of triples */
			return tac_connect(tlist, tnode);
		}

		case AST_WHILE:
		{

			// Create a label for the bexpr and connect it to the actual tlist
			tac_node * start_bexpr = _tac_label();
			tlist = tac_connect(tlist, start_bexpr);

			// Create a label that points to the end of the body
			tac_node * end_while_label = _tac_label();

			/* Bexpr */
			{
				tac_value * bexpr = _tac_leaf(node->ast_children[0]);

				if(bexpr != NULL){ //It is a leaf
					tac_node * node = _tac_node();
					node->value->op = TAC_NOT;
					node->value->arg0 = bexpr;
					tlist = tac_connect(tlist, node);
				} else {
					tac_list * bexpr = tac_ast_node(node->ast_children[0]);
					tac_node * negateCondition = _tac_node();
					negateCondition->value->op = TAC_NOT;
					negateCondition->value->arg0 = calloc(1, sizeof(tac_value));
					negateCondition->value->arg0->instruction = bexpr->last->value;
					tlist = tac_append(tlist, bexpr);
					tlist = tac_connect(tlist, negateCondition);
				}
			}

			//If the negate condition is true goto end of while
			tac_node * goto_skip_while_body = _tac_goto_conditioned(tlist->last->value, end_while_label);
			tlist = tac_connect(tlist, goto_skip_while_body);

			//Calculate the code for the body of the while and append it to the tlist */
			tac_list * stmt = generate_tac(node->seq_children[0]);
			tlist = tac_append(tlist, stmt);

			// generate goto node that points to the start_bexpr label
			tac_node * goto_bexpr_evaluation = _tac_goto_unconditioned(start_bexpr);
			tlist = tac_connect(tlist, goto_bexpr_evaluation);

			/* Add the label to exit from the while loop */
			tlist = tac_connect(tlist, end_while_label);

			return tlist;
		}
		case AST_IF:
		{
			/* Treating bexpr - in a similar way as while */
			{
				tac_value * bexpr = _tac_leaf(node->ast_children[0]);

				if(bexpr != NULL){ //It is a leaf
					tac_node * node = _tac_node();
					node->value->op = TAC_NOT;
					node->value->arg0 = bexpr;
					tlist = tac_connect(tlist, node);
				} else {
					tac_list * bexpr = tac_ast_node(node->ast_children[0]);
					tac_node * negateCondition = _tac_node();
					negateCondition->value->op = TAC_NOT;
					negateCondition->value->arg0 = calloc(1, sizeof(tac_value));
					negateCondition->value->arg0->instruction = bexpr->last->value;
					tlist = tac_append(tlist, bexpr);
					tlist = tac_connect(tlist, negateCondition);
				}
			}

			if(node->number_of_seq_children == 1){ //IF
				tac_node * end_if_body = _tac_label();
				// Create a conditioned goto that onTrue of the negate condition goes to end_while_label
				tac_node * goto_skip_if_body = _tac_goto_conditioned(tlist->last->value, end_if_body);
				//calculate tlist of the stmt following bexpr
				tac_list * stmt = generate_tac(node->seq_children[0]);
				tlist = tac_connect(tlist, goto_skip_if_body);
				tlist = tac_append(tlist, stmt);
				tlist = tac_connect(tlist, end_if_body);
			} else if(node->number_of_seq_children == 2){ //IF THEN ELSE
				tac_node * start_else_body = _tac_label();
				tac_node * end_else_body = _tac_label();
				tac_node * goto_start_else_body = _tac_goto_conditioned(tlist->last->value, start_else_body);
				tac_list * if_body = generate_tac(node->seq_children[0]);
				tac_node * goto_end_else_body = _tac_goto_unconditioned(end_else_body);
				tac_list * else_body = generate_tac(node->seq_children[1]);

				tlist = tac_connect(tlist, goto_start_else_body);
				tlist = tac_append(tlist, if_body);
				tlist = tac_connect(tlist, goto_end_else_body);
				tlist = tac_connect(tlist, start_else_body);
				tlist = tac_append(tlist, else_body);
				tlist = tac_connect(tlist, end_else_body);
			} else {
				char * s = malloc(sizeof(char) * strlen(__FILE__) + 1);
				strcpy(s, __FILE__);
				yyerror("%s: IF with more than two children, not yet supported", s);
			}
			return tlist;
			break;
		}
		/* Leaves */
		case AST_PRINT: /* one child subtree */
		{
			return tac_connect(tlist, _tac_print(node));
		}
		case AST_DECLARATION:
			return tlist;
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
	node = calloc(1, sizeof(tac_node));
	node->value = calloc(1, sizeof(tac_entry));
	node->value->op = -1;
	node->value->arg0 = NULL;
	node->value->arg1 = NULL;
	node->prev=NULL;
	node->next=NULL;
	return node;
}


static
tac_value * _tac_leaf(ast_node * node){
	tac_value * val = NULL;
	switch(node->data->token){
		case AST_FRACT:
		{
			val = calloc(1,sizeof(tac_value));
			val->fract = (fract_t*) node->data->value;
			break;
		}
		case AST_BOOL:
		{
			val = calloc(1,sizeof(tac_value));
			val->boolean =  *(bool*) node->data->value;
			break;
		}
		case AST_ID:
		{
			val = calloc(1,sizeof(tac_value));
			val->address = (symbol_table_entry *)
				lookupID(node->data->value);
			break;
		}
		default : break;
	}

	return val;
}

static
tac_node * _tac_print(ast_node * node){
	tac_node * tnode=_tac_node();
	tnode->value->op= TAC_PRINT;
	tnode->value->arg0= calloc(1, sizeof(tac_value));
	tnode->value->arg0->address=
		(symbol_table_entry *) lookupID(node->ast_children[0]->data->value);
	return tnode;
}
static
tac_node * _tac_label(){
	tac_node * label = _tac_node();
	label->value->op = TAC_LABEL;
	label->value->arg0=NULL;
	label->value->arg1=NULL;
	label->prev=NULL;
	label->next=NULL;
	return label;
}

static
tac_node * _tac_goto_unconditioned(tac_node * destination){
	tac_node * goto_node=_tac_node();
	goto_node->value->op=TAC_GOTO;
	goto_node->value->arg0=calloc(1, sizeof(tac_value));
	goto_node->value->arg0->instruction=destination->value;
	return goto_node;
}
static
tac_node * _tac_goto_conditioned(tac_entry * condition, tac_node * destination){
	tac_node * goto_node=_tac_node();
	goto_node->value->op=TAC_GOTO;
	goto_node->value->arg0=calloc(1, sizeof(tac_value));
	goto_node->value->arg0->instruction=condition;
	goto_node->value->arg1=calloc(1, sizeof(tac_value));
	goto_node->value->arg1->instruction=destination->value;
	return goto_node;
}
