#include "factype.h"
#include "factype_ast.h"
#include "type_checker.h"
#include "symbol_table.h"
#include "parser.tab.h"
#include <stdio.h>
extern entry * symbol_table;

bool recursive_type_checking_fract(AST_node * node){
	switch(node->data->token){
		case FRACT: return true;
		case ID: return getType((char*) node->data->value) == FRACT_T;
		case AST_AOP:
			if(node->number_of_children == 1){
				return recursive_type_checking_fract(node->children[0]);
			}
			else {
				return recursive_type_checking_fract(node->children[0]) &&
						recursive_type_checking_fract(node->children[1]);
			}
		default:
			return false;
	}
}

bool recursive_type_checking_bool(AST_node * node){
	switch(node->data->token){
		case BOOL: return true;
		case ID: return getType((char*) node->data->value) == BOOL_T;
		case BOP1: return recursive_type_checking_bool(node->children[0]);
		case AST_BOP2:
			return recursive_type_checking_bool(node->children[0]) &&
				recursive_type_checking_bool(node->children[1]);
		case AST_RELOP:
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
		default: fprintf(stderr, "Type not recognized\n"); exit(EXIT_FAILURE); break;
	}
	if(!res){
		fprintf(stderr, "Type Mismatch in instruction + %s!\n", type==FRACT_T?"FRACT":"BOOL");
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
			//equivalent to checking it the type is already defined
			getType(node->children[0]->data->value);
			break;
		}
	}

	if(success == true){
		printf("Type checking successful!\n");
	} 
	return success;
}

bool type_checking(seq_node * seqTree){
	if(seqTree == NULL){
		return true;
	}
	bool res = type_checking(seqTree->left);
	
	res &= type_checking_AST_node(seqTree->right);
	printf("%s\n", res?"true":"false");
	return res;
}
