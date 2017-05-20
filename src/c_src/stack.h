/**
 * @file 
 * @brief definition and API of a stack
 * 
 * @author <mirko.bez@studenti.unipd.it>
 * @author <stefano.munari.1@studenti.unipd.it>
 */
#ifndef __STACK_H__
#define __STACK_H__
#define STACK_MAX 100
#include <stdbool.h>

typedef struct stack_t {
    void * data[STACK_MAX];
    int size;
} stack_t;

void init(stack_t * s);
void * top(stack_t * s);
void push(stack_t * s, void * data);
void pop(stack_t * s);
bool isEmpty(stack_t * s);

#endif /*__STACK_H__*/
