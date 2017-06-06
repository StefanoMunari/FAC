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
* @brief builds the 3AC list of triples from the last to the first node of the
* 		given AST by traversing the AST bottom-up
* @note opt_t and tac_op have the same value because the enums follow
* 		the same declaration order
* @note do not free the goto NULL NULL node, reuse it to keep consistency
*/
tac_list * tac_ast_node(ast_node * node){
	if(node == NULL || node->data == NULL)
		yyerror("tac_ast_node::tac_ast_node: \
				TAC - malformed AST, null node found");
	/* Create an empty tlist */
	tac_list* tlist = calloc(1, sizeof(tac_list));
	/* Process an AST node */
	switch(node->data->token){
		/* Internal nodes */
		case AST_AOP1:
		case AST_BOP1:
		{/* Unary Operators */
			/* create a new node 3AC node */
			tac_node* tnode=_tac_node();
			tnode->value->op = node->data->op;
			tac_value * left = _tac_leaf(node->ast_children[0]);
			/* left is a leaf */
			if(left != NULL){
				tnode->value->arg0 = left;
			} else {/* left is a tree with height > 0 */
				tac_list * left_list = tac_ast_node(node->ast_children[0]);
				tnode->value->arg0= calloc(1, sizeof(tac_value));
				tnode->value->arg0->instruction= left_list->last->value;
				/* append the current computed list to the 3AC list */
				tlist = tac_append(tlist, left_list);
			}
			/* connect tnode to the current list of triples */
			return tac_connect(tlist, tnode);
		}
		case AST_AOP2:
		case AST_BOP2:
		case AST_BOP_RELOP:
		case AST_RELOP:
		{/* Binary Operators */
			/* create a new node 3AC node */
			tac_node* tnode=_tac_node();
			tnode->value->op = node->data->op;
			/* compute the left-subtree in 3AC */
			tac_value * left = _tac_leaf(node->ast_children[0]);
			/* left is a leaf */
			if(left != NULL){
				tnode->value->arg0 = left;
			} else {/* left is a tree with height > 0 */
				tac_list* left = tac_ast_node(node->ast_children[0]);
				tnode->value->arg0= calloc(1, sizeof(tac_value));
				tnode->value->arg0->instruction= left->last->value;
				tlist = tac_append(tlist, left);
			}
			/* compute the right-subtree in 3AC */
			tac_value * right = _tac_leaf(node->ast_children[1]);
			/* right is a leaf */
			if(right != NULL){
				tnode->value->arg1 = right;
			} else {/* right is a tree with height > 0 */
				tac_list* right = tac_ast_node(node->ast_children[1]);
				tnode->value->arg1= calloc(1, sizeof(tac_value));
				tnode->value->arg1->instruction= right->last->value;
				tlist = tac_append(tlist, right);
			}
			/* connect tnode to the current list of triples */
			return tac_connect(tlist, tnode);
		}
		case AST_ASSIGNMENT:
		{/* Assignment Operator */
			/* create a new node 3AC node */
			tac_node* tnode=_tac_node();
			tnode->value->op = TAC_ASSIGNMENT;
			tnode->value->arg0 = calloc(1, sizeof(tac_value));
			tnode->value->arg0->address = (symbol_table_entry*)
				lookupID(node->ast_children[0]->data->value);
			/* Compute the the right hand side in 3AC */
			tac_value * leaf = _tac_leaf(node->ast_children[1]);
			/* the right hand side is a leaf */
			if(leaf != NULL){
				tnode->value->arg1 = leaf;
			} else {/* the right hand side is a tree with height > 0 */
				tac_list* right = tac_ast_node(node->ast_children[1]);
				tnode->value->arg1 = calloc(1, sizeof(tac_value));
				tnode->value->arg1->instruction =  right->last->value;
				tlist = tac_append(tlist, right);
			}
			/* connect tnode to the current list of triples */
			return tac_connect(tlist, tnode);
		}
		case AST_WHILE:
		{/* While Statement */
			/* Create a label for bexpr and connect it to the actual tlist */
			tac_node * start_bexpr = _tac_label();
			tlist = tac_connect(tlist, start_bexpr);
			/* Create a label that points to the end of the body of the while */
			tac_node * end_while_label = _tac_label();
			/* Compute the bexpr in 3AC */
			{
				tac_value * bexpr = _tac_leaf(node->ast_children[0]);
				/* bexpr is a leaf */
				if(bexpr != NULL){
					tac_node * node = _tac_node();
					node->value->op = TAC_NOT;
					node->value->arg0 = bexpr;
					tlist = tac_connect(tlist, node);
				} else {/* bexpr is a tree with height > 0 */
					tac_list * bexpr = tac_ast_node(node->ast_children[0]);
					tac_node * negateCondition = _tac_node();
					negateCondition->value->op = TAC_NOT;
					negateCondition->value->arg0 = calloc(1, sizeof(tac_value));
					negateCondition->value->arg0->instruction =
						bexpr->last->value;
					tlist = tac_append(tlist, bexpr);
					tlist = tac_connect(tlist, negateCondition);
				}
			}
			/* If the negate condition is true goto end of while */
			tac_node * goto_skip_while_body =
				_tac_goto_conditioned(tlist->last->value, end_while_label);
			tlist = tac_connect(tlist, goto_skip_while_body);
			/* Generate code for the body of the while and
				append it to the tlist */
			tac_list * stmt = generate_tac(node->seq_children[0]);
			tlist = tac_append(tlist, stmt);
			/* Generate the goto node which points to the start_bexpr label */
			tac_node * goto_bexpr_evaluation =
					_tac_goto_unconditioned(start_bexpr);
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
					negateCondition->value->arg0->instruction =
						bexpr->last->value;
					tlist = tac_append(tlist, bexpr);
					tlist = tac_connect(tlist, negateCondition);
				}
			}

			if(node->number_of_seq_children == 1){ //IF
				tac_node * end_if_body = _tac_label();
				/* Create a conditioned goto that onTrue of the negate condition
				 * goes to end_while_label*/
				tac_node * goto_skip_if_body =
						_tac_goto_conditioned(tlist->last->value, end_if_body);
				//calculate tlist of the stmt following bexpr
				tac_list * stmt = generate_tac(node->seq_children[0]);
				tlist = tac_connect(tlist, goto_skip_if_body);
				tlist = tac_append(tlist, stmt);
				tlist = tac_connect(tlist, end_if_body);
			} else if(node->number_of_seq_children == 2){ //IF THEN ELSE
				tac_node * start_else_body = _tac_label();
				tac_node * end_else_body = _tac_label();
				tac_node * goto_start_else_body =
							_tac_goto_conditioned(tlist->last->value,
													start_else_body
							);
				tac_list * if_body = generate_tac(node->seq_children[0]);
				tac_node * goto_end_else_body =
						_tac_goto_unconditioned(end_else_body);
				tac_list * else_body = generate_tac(node->seq_children[1]);

				tlist = tac_connect(tlist, goto_start_else_body);
				tlist = tac_append(tlist, if_body);
				tlist = tac_connect(tlist, goto_end_else_body);
				tlist = tac_connect(tlist, start_else_body);
				tlist = tac_append(tlist, else_body);
				tlist = tac_connect(tlist, end_else_body);
			} else {
				yyerror("tac_ast_node::tac_ast_node:\
						IF with more than two children, not yet supported"
				);

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
			yyerror("tac_ast_node::tac_ast_node: token not recognized");
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
	node->prev = NULL;
	node->next = NULL;
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
	tac_node * tnode =_tac_node();
	tnode->value->op = TAC_PRINT;
	tnode->value->arg0 = calloc(1, sizeof(tac_value));
	tnode->value->arg0->address =
		(symbol_table_entry *) lookupID(node->ast_children[0]->data->value);
	return tnode;
}
static
tac_node * _tac_label(){
	tac_node * label = _tac_node();
	label->value->op = TAC_LABEL;
	label->value->arg0 = NULL;
	label->value->arg1 = NULL;
	label->prev = NULL;
	label->next = NULL;
	return label;
}

static
tac_node * _tac_goto_unconditioned(tac_node * destination){
	tac_node * goto_node =_tac_node();
	goto_node->value->op = TAC_GOTO;
	goto_node->value->arg0 = calloc(1, sizeof(tac_value));
	goto_node->value->arg0->instruction = destination->value;
	return goto_node;
}
static
tac_node * _tac_goto_conditioned(tac_entry * condition, tac_node * destination){
	tac_node * goto_node = _tac_node();
	goto_node->value->op = TAC_GOTO;
	goto_node->value->arg0 = calloc(1, sizeof(tac_value));
	goto_node->value->arg0->instruction = condition;
	goto_node->value->arg1 = calloc(1, sizeof(tac_value));
	goto_node->value->arg1->instruction = destination->value;
	return goto_node;
}
