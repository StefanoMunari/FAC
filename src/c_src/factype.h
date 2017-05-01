#ifndef _FACTYPE_H_
#define _FACTYPE_H_
#include <stdbool.h>

typedef struct fract_t
{
	int num;
	int den;
} fract_t;

typedef struct value_t
{
	fract_t fract_val;
	bool bool_val;
	
} value_t;

typedef enum 
{
	SUM,
	DIFF	
} aop0_t;

typedef enum 
{
	MULT,
	DIV	
} aop1_t;

typedef enum
{ 
	NOT
} bop1_t;

typedef enum
{ 
	IFF, 
	AND, 
	OR, 
	IMPLY, 
	XOR
} bop2_t;

typedef enum
{
	LT, 
	LEQ, 
	EQ, 
	GEQ, 
	GT, 
	NEQ
} relop_t;

typedef union {
	aop0_t aop0;
	aop1_t aop1;
	bop1_t bop1;
	bop2_t bop2;
	relop_t relop;
} op_t;


typedef enum 
{
	BOOL_T,
	FRACT_T	
} type_t;




#endif/*_FACTYPE_H_*/
