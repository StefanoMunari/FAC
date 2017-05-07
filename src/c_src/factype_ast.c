#include "factype_ast.h"
#include "AST.h"
#include "seq_tree.h"
#include "factype.h"
#include "parser.tab.h" //Include the tokens
#include <assert.h>
#include <stdlib.h>
/**
* @brief exploit the enum representation of parser's types to compute
*	the corresponding AST arithmetic type representation
* @see factype.h for arithmetic types used in the parser
* @see factype_ast.h for arithmetic types used in the AST
* @see parser.tab.h for the values used in the 'case' statements
*/
aop_t aop(int aop_type, int aop_value){
	assert((aop_value >= 0) && (aop_value <= 1));
	switch(aop_type){
		case AOP_0:
			return aop_value;
			break;
    	case AOP_1:
    	//add the offset given by the previous enum (AOP_0)
    		return aop_value+2;
    		break;
    	default:
			assert(false);
	}
}
/**
* @brief exploit the enum representation of parser's types to compute
*	the corresponding AST boolean type representation
* @see factype.h for boolean types used in the parser
* @see factype_ast.h for boolean types used in the AST
* @see parser.tab.h for the values used in the 'case' statements
*/
bop2_t bop2(int bop2_type, int bop2_value){
	switch(bop2_type){
		case BOP2_0:
		//save the assert checking since it can assume only 1 value
			return 0;
			break;
    	case BOP2_1:
    		return 1;
    		break;
		case BOP2_2:
			return 2;
			break;
    	case BOP2_3:
			assert((bop2_value >= 0) && (bop2_value <= 1));
			//add the offset given by the previous enums
    		return bop2_value+3;
			break;
    	default:
			assert(false);
	}
}

relop_t relop(int relop_type, int relop_value){
	return 0;
}


