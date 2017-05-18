#ifndef __TAC_LIST_H__
#define __TAC_LIST_H__
#include "tac.h"

typedef struct tac_node {
	struct tac_entry * value;
	struct tac_node * prev;
	struct tac_node * next;
} tac_node;

typedef struct tac_list{
	struct tac_node * first;
	struct tac_node * last;
} tac_list;

#endif /*__TAC_LIST_H__*/