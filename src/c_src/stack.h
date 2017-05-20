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
/**
 * Returns the value on top of the stack without removing it from the stack 
 * @param s the stack
 * @return the value on top of the stack
 */
void * top(stack_t * s);
/**
 * Push data on top of the stack
 * @param s the stack
 * @param data the data to put on the stack
 */
void push(stack_t * s, void * data);
/**
 * Pop an element from the stack and return it
 * @param s the stack
 */
void * pop(stack_t * s);
/**
 * @param s the stack
 * @return true if s is empty otherwise false
 */
bool isEmpty(stack_t * s);

#endif /*__STACK_H__*/
