#include "tac_ast_node.h"
#include "factype_ast.h"
#include "factype_tac.h"
#include "tac.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
extern void yyerror(const char *, ...);

static
tac_node * _tac_node();
static
tac_list * _tac_connect(tac_list *, tac_node *);
static
tac_list * _tac_append(tac_list *, tac_list *);
static
tac_node * _tac_label();
static
tac_node * _tac_goto();
static
tac_list * _tac_fract(ast_node *);
static
tac_list * _tac_bool(ast_node *);
static
tac_list * _tac_id(ast_node *);
static
tac_node * _tac_print(ast_node *);
int check=0;
/**
* @brief builds the 3AC list of triples from the last to the first node by
*	traversing the AST bottom-up
* @note opt_t and tac_op have the same value because the enums follow
* 		the same declaration order
* @note do not free the goto NULL NULL node, reuse it to keep consistency
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
			/* compute left subtree -
			   NOTE: it also connects it to the current list of triples */
			tac_list* left=tac_ast_node(node->ast_children[0], tlist, stack);
			/* 3AC - expression */
			if(left->last->value->op != -1){
				/* subtree with height > 0 */
				tnode=_tac_node();
				tnode->value->arg0= calloc(1, sizeof(tac_value));
				tnode->value->arg0->instruction= left->last->value;
			}
			else/* a leaf */
			{
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
			/* current node */
			tac_node* tnode=_tac_node();
			/* compute right subtree -
			   NOTE: it also connects it to the current list of triples */
			tac_list* right=tac_ast_node(node->ast_children[1], tlist, stack);
			tac_node* right_node=right->last;
			/* compute left subtree -
			   NOTE: also connects it to the current list of triples */
			tac_list* left=tac_ast_node(node->ast_children[0], tlist, stack);
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
			printf("ASS : %d\n", check);
			++check;
			tac_node* tnode=NULL;
			/* compute right subtree -
			   NOTE: it also connects it to the current list of triples */
			tac_list* right=tac_ast_node(node->ast_children[1], tlist, stack);
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
			printf("END - ASS : %d\n", check);
			/* connect tnode to the current list of triples */
			return _tac_connect(tlist, tnode);
		}
		case AST_WHILE:
		{
			/* Create a label for the bexpr and connect it to the actual tlist */
			tac_node * start_bexpr = _tac_label();
			tlist = _tac_connect(tlist, start_bexpr);
			
			/* Create 3AC for the bexpr and append to the actual tlist */
			tlist=tac_ast_node(node->ast_children[0], tlist, stack);
			
			/* adjust the bexpr - if it is a leaf */
			if(!tlist->last->value->arg0){
				tlist->last->value->op=TAC_COND;
				tlist->last->value->arg0=tlist->last->value->arg1;
				tlist->last->value->arg1=NULL;
			}
			
			/* generate stmt tac_node */
			tac_list * stmt = generate_tac(node->seq_children[0]);
			
			/* generate goto node that points to the start_bexpr label */
			tac_node * goto_node = _tac_goto();
			goto_node->value->arg0 = calloc(1, sizeof(tac_value));
			goto_node->value->arg0->instruction=start_bexpr->value;
			
			tlist = _tac_append(tlist, stmt);
			tlist = _tac_connect(tlist, goto_node);
			return tlist;
		}
		case AST_IF:
		{
			/* Calculate list containing tlist extended with bexpr code */
			tlist=tac_ast_node(node->ast_children[0], tlist, stack);
			/* adjust the bexpr - if it is a leaf */
			if(!tlist->last->value->arg0){
				tlist->last->value->op=TAC_COND;
				tlist->last->value->arg0=tlist->last->value->arg1;
				tlist->last->value->arg1=NULL;
			}
			/* calculate tlist of the stmt following bexpr */
			tac_list * stmt=generate_tac(node->seq_children[0]);
			
			/* Create the two labels */
			tac_node * true_node=_tac_label();
			tac_node * end_node=_tac_label();
			
			/* Create the goto entry */
			tac_node * goto_node=_tac_goto();
			
			/* set up goto node using labels */
			goto_node->value->arg0 = calloc(1, sizeof(tac_value));
			goto_node->value->arg0->instruction=true_node->value;
			/* branch false - skip the end list (reference the end label) */
			goto_node->value->arg1 = calloc(1, sizeof(tac_value));
			goto_node->value->arg1->instruction=end_node->value;
			/* connect label + stmt to the current list of triples -
				true is already connected */
			
			return
				_tac_append(
					_tac_connect(
						_tac_connect(tlist, goto_node),
						true_node),
					_tac_connect(stmt, end_node));
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
	label->value->arg0=NULL;
	label->value->arg1=NULL;
	label->prev=NULL;
	label->next=NULL;
	return label;
}

static
tac_node * _tac_goto(){
	tac_node * goto_node=_tac_node();
	goto_node->value->op=TAC_GOTO;
	goto_node->value->arg0=NULL;
	goto_node->value->arg1=NULL;
	goto_node->prev=NULL;
	goto_node->next=NULL;
	return goto_node;
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
		printf("CONNECT : %p -> %p\n", tlist->last->value, tnode->value);
		tlist->last->next=tnode;
		tnode->prev=tlist->last;
		tlist->last=tlist->last->next;
	}
	return tlist;
}

static
tac_list * _tac_append(tac_list * tlist, tac_list * to_append){
	if(!to_append->first && !to_append->last)
		return tlist;
	if(!tlist->last && !tlist->first){
		/* first node of the TAC list - nothing to connect */
		tlist->first=to_append->first;
		tlist->last=to_append->last;
		return tlist;
	}
	tlist->last->next=to_append->first;
	to_append->first->prev=tlist->last;
	tlist->last=to_append->last;
	to_append->first=tlist->first;
	return tlist;
}

static
tac_list * _tac_fract(ast_node * node){
	tac_list * tlist=malloc(sizeof(tac_list));
	tlist->last=_tac_node();
	tlist->first=tlist->last;
	/* convention - set always arg1 */
	tlist->last->value->arg1 = calloc(1, sizeof(tac_value));
	tlist->last->value->arg1->fract = (fract_t*) node->data->value;
	return tlist;
}

static
tac_list * _tac_bool(ast_node * node){
	tac_list * tlist=malloc(sizeof(tac_list));
	tlist->last=_tac_node();
	tlist->first=tlist->last;
	/* convention - set always arg1 */
	tlist->last->value->arg1 = calloc(1, sizeof(tac_value));
	tlist->last->value->arg1->boolean = node->data->value;
	return tlist;
}

static
tac_list * _tac_id(ast_node * node){
	tac_list * tlist=malloc(sizeof(tac_list));
	tlist->last=_tac_node();
	tlist->first=tlist->last;
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
