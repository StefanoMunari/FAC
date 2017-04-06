#ifndef _FACTYPE_H_
#define _FACTYPE_H_

struct fract_t
{
	int num;
	int den;
};
typedef struct fract_t fract_t;

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

typedef enum 
{
	BOOL,
	FRACT	
} type_t;

#endif/*_FACTYPE_H_*/