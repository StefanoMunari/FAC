#include "factype.h"
#include "type_checker.h"
#include "symbol_table.h"
#include "parser.tab.h"
#include <stdio.h>
extern entry * symbol_table;

bool recursive_type_checking_fract(AST_node * node){
	switch(node->data->token){
		case FRACT: return true;
		case ID: return getType((char*) node->data->value) == FRACT_T;
		case AOP_0:
			if(node->number_of_children == 1){
				return recursive_type_checking_fract(node->children[0]);
			}
			else {
				return recursive_type_checking_fract(node->children[0]) &&
						recursive_type_checking_fract(node->children[1]);
			}
		case AOP_1: return recursive_type_checking_fract(node->children[0]) &&
						recursive_type_checking_fract(node->children[1]);
		default: return false;
	}
}

bool recursive_type_checking_bool(AST_node * node){
	switch(node->data->token){
		case BOOL: return true;
		case ID: return getType((char*) node->data->value) == BOOL_T;
		case BOP1: return recursive_type_checking_bool(node->children[0]);
		case BOP2_0:
		case BOP2_1:
		case BOP2_2:
		case BOP2_3:
			return recursive_type_checking_bool(node->children[0]) &&
				recursive_type_checking_bool(node->children[1]);
		case RELOP_0:
		case RELOP_1:
			return recursive_type_checking_fract(node->children[0]) &&
				recursive_type_checking_fract(node->children[1]);
		default: return false;

	}
}
/**
 * Check if the given expr has the right type
 */
bool recursive_type_checking(AST_node * AST, type_t type){
	bool res;
	switch(type){
		case FRACT_T: res = recursive_type_checking_fract(AST); break;
		case BOOL_T: res = recursive_type_checking_bool(AST); break;
	}
	if(!res){
		fprintf(stderr, "Type Mismatch!\n");
		printASTNode(AST);

	}
	return res;
}


bool type_checking_AST_node(AST_node * AST) {
	
	AST_node * node = AST;
	bool success = true;
	switch(node->data->token){
		 case TYPE: /* Perform a declaration */
			printf("TYPE\n");
			installID((char*) node->children[0]->data->value, node->data->type);
			break;
		 case ASSIGNMENT:
		 { /* Perform an assignment */
			type_t expected = getType(node->children[0]->data->value);
			success &= recursive_type_checking(node->children[1], expected);
			break;
		 }
		case PRINT:
		{ /* Check only if the ID is installed in the symbol table */
			printf("PRINT\n");
			type_t expected = getType(node->children[0]->data->value);
			break;
		}
	}

	if(success == true){
		printf("Type checking successful!\n");
	}
}

bool type_checking(seq_node * seqTree){
	if(seqTree == NULL){
		return true;
	}
	type_checking(seqTree->left);
	printf("Perform my own type checking ...\n");
	type_checking_AST_node(seqTree->right);
}
