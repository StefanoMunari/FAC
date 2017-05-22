#include "tac_ast_node.h"
#include "factype_ast.h"
#include "factype_tac.h"
#include "tac.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
extern void yyerror(char *, ...);

static
tac_node * _tac_label();
static
tac_node * _tac_node();
static
tac_list * _tac_connect(tac_list *, tac_node *);
static
tac_list * _tac_append(tac_list * tlist, tac_list * Append);
static
tac_list * _tac_fract(ast_node *);
static
tac_list * _tac_bool(ast_node *);
static
tac_list * _tac_id(ast_node *);
static
tac_node * _tac_print(ast_node *);
static
tac_node * _tac_unconditioned_goto(tac_node * destination);
static
tac_node * _tac_conditioned_goto(tac_node * condition,  tac_node * destination);
int check=0;
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
			/* compute left subtree -
			   NOTE: it also connects it to the current list of triples */
			tac_list* left=tac_ast_node(node->ast_children[0], tlist, stack);
			tac_node* tnode=NULL;
			/* 3AC - expression */
			if(left->last->value->op != -1){
				/* subtree with height > 0 */
				tnode=_tac_node();
				tnode->value->arg0= calloc(1, sizeof(tac_value));
				tnode->value->arg0->instruction= left->last->value;
			}
			else/* a leaf */{
				tnode=left->last;
				tnode->value->arg0=tnode->value->arg1;
			}
			/* complete tnode setup */
			tnode->value->op = node->data->op;
			tnode->value->arg1=NULL;
			/* connect tnode to the current list of triples */
			return _tac_connect(tlist, tnode);
		}
		case AST_AOP2:
		case AST_BOP2:
		case AST_RELOP:
		{
			/* compute right subtree -
			   NOTE: it also connects it to the current list of triples */
			tac_list* right=tac_ast_node(node->ast_children[1], tlist, stack);
			tac_node* right_node=right->last;
			/* compute left subtree -
			   NOTE: also connects it to the current list of triples */
			tac_list* left=tac_ast_node(node->ast_children[0], tlist, stack);
			/* current node */
			tac_node* tnode=_tac_node();
			/* 3AC - left operand */
			if(left->last->value->op != -1){
				/* subtree with height > 0 */
				tnode->value->arg0 = calloc(1, sizeof(tac_value));
				tnode->value->arg0->instruction = left->last->value;
			}
			else/* a leaf */
				tnode->value->arg0=left->last->value->arg1;
			/* 3AC - right operand */
			if(right->last->value->op != -1){
				/* subtree with height > 0 */
				tnode->value->arg1 = calloc(1, sizeof(tac_value));
				tnode->value->arg1->instruction = right_node->value;
			}
			else /* a leaf */
				tnode->value->arg1=right->last->value->arg1;
			/* complete tnode setup */
			tnode->value->op = node->data->op;
			/* connect tnode to the current list of triples */
			return _tac_connect(tlist, tnode);
		}
		case AST_ASSIGNMENT:
		{
			/* compute right subtree -
			   NOTE: it also connects it to the current list of triples */
			tac_list* right=tac_ast_node(node->ast_children[1], tlist, stack);
			tac_node* tnode=NULL;
			/* 3AC - right side of the assignment */
			if(right->last->value->op != -1){
				/* subtree with height > 0 */
				tnode=_tac_node();
				tnode->value->arg1 = calloc(1, sizeof(tac_value));
				tnode->value->arg1->instruction =  right->last->value;
			}
			else/* a leaf */
				tnode=right->last;
			/* complete tnode setup */
			tnode->value->op = TAC_ASSIGNMENT;
			/* left side of the assignment */
			tnode->value->arg0 = calloc(1, sizeof(tac_value));
			tnode->value->arg0->address =
				lookupID(node->ast_children[0]->data->value);
			/* connect tnode to the current list of triples */
			return _tac_connect(tlist, tnode);
		}
		case AST_IF:
		{
			if(node->number_of_ast_children == 1){
				/* Calculate list containing old code and bexpr code */
				tac_ast_node(node->ast_children[0], tlist, stack);
				/* calculate tlist of the stmt */
				tac_list * stmt = generate_tac(node->SEQ_children[0]);
				assert(stmt != NULL);
				assert(stmt->last != NULL);
				assert(stmt->first != NULL);
				
				/* Create the two labels */
				tac_node * startBranchLabel = _tac_label();
				tac_node * endBranchLabel = _tac_label();
				
				
				/* Initialize a conditioned goto stmt. If true goto stmt */
				tac_node * gotoSTMT = _tac_conditioned_goto(tlist->last, startBranchLabel);
				
				/* Initialize a not conditioned goto stmt, that corresponds to
				 * if false goto end of branch
				 */
				tac_node * gotoEndLabel = _tac_unconditioned_goto(endBranchLabel);
				
				
				/* Append the created lists and nodes */
				_tac_connect(tlist, gotoSTMT);
				_tac_connect(tlist, gotoEndLabel);
				_tac_connect(tlist, startBranchLabel);
				_tac_append(tlist, stmt);
				_tac_connect(tlist, endBranchLabel);
				
				return tlist;
			} else {
				/* Calculate list containing old code and bexpr code */
				tac_ast_node(node->ast_children[0], tlist, stack);

				/* calculate tlist of the stmt */
				tac_list * stmt1 = generate_tac(node->SEQ_children[0]);
				tac_list * stmt2 = generate_tac(node->SEQ_children[1]);
				
				/* Create the two labels */
				tac_node * startBranchLabel = _tac_label();
				tac_node * elseBranchLabel = _tac_label();
				tac_node * endBranchLabel = _tac_label();
				
				
				/* Initialize a conditioned goto stmt. If true goto stmt */
				tac_node * gotoSTMT1 = _tac_conditioned_goto(tlist->last, startBranchLabel);
				
				
				/* Initialize a not conditioned goto stmt, that corresponds to
				 * if false goto end of branch
				 */
				tac_node * gotoElseLabel = _tac_unconditioned_goto(elseBranchLabel);
				
				tac_node * gotoEndLabel = _tac_unconditioned_goto(endBranchLabel);
				
				
				/* Append the created lists and nodes */
				
				_tac_connect(tlist, gotoSTMT1);
				_tac_connect(tlist, gotoElseLabel);
				_tac_connect(tlist, startBranchLabel);
				_tac_append(tlist, stmt1);
				_tac_connect(tlist, gotoEndLabel);
				_tac_connect(tlist, elseBranchLabel);
				_tac_append(tlist, stmt2);
				_tac_connect(tlist, endBranchLabel);
				
				return tlist;
			
			}
			
			return tlist;
		}
		/* Leaves */
		case AST_FRACT:
			return _tac_fract(node);
		case AST_BOOL:
			return _tac_bool(node);
		case AST_ID:
			return _tac_id(node);
		case AST_PRINT: /* one child subtree */
			return _tac_connect(tlist, _tac_print(node));
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
tac_node * _tac_label(){
	tac_node * label = _tac_node();
	label->value->op = TAC_LABEL;
	return label;
}

static
tac_node * _tac_unconditioned_goto(tac_node * destination){
	tac_node * gotoNode = _tac_node();
	gotoNode->value->op = TAC_GOTO;
	gotoNode->value->arg0 = malloc(sizeof(tac_value));
	gotoNode->value->arg0->instruction = destination->value;
	return gotoNode;
}

static
tac_node * _tac_conditioned_goto(tac_node * condition,  tac_node * destination){
	tac_node * gotoNode = _tac_node();
	gotoNode->value->op = TAC_GOTO;
	gotoNode->value->arg0 = malloc(sizeof(tac_value));
	gotoNode->value->arg0->instruction = condition->value;
	gotoNode->value->arg1 = malloc(sizeof(tac_value));
	gotoNode->value->arg1->instruction = destination->value;
	return gotoNode;
}

static
tac_list * _tac_connect(tac_list * tlist, tac_node * tnode){
	
	if(!tlist->last && !tlist->first){
		/* first node of the TAC list - nothing to connect */
		tlist->first=tnode;
		tlist->last=tnode;
	}
	else{
		if(tnode == NULL)
			return tlist;
		tlist->last->next=tnode;
		tnode->prev=tlist->last;
		tlist->last=tlist->last->next;
	}
	return tlist;
}

static
tac_list * _tac_append(tac_list * tlist, tac_list * Append){
	
	tlist->last->next = Append->first;
	Append->first->prev = tlist->last;
	tlist->last = Append->first;
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
	/* convention - set always arg1 */
	tlist->last->value->arg1= calloc(1, sizeof(tac_value));
	tlist->last->value->arg1->address=
		(symbol_table_entry *) lookupID(node->data->value);
	return tlist;
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
