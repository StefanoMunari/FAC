#ifndef __TAC_LIST_H__
#define __TAC_LIST_H__
#include "tac.h"


typedef struct tac_node {
	tac_entry * value;
	tac_node * prev;
	tac_node * next;
} tac_node;

#endif /*__TAC_LIST_H__*/