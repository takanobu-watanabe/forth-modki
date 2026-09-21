#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

typedef struct Stack Stack;

Stack* stack_new(int capacity);
void stack_free(Stack *stack);
void stack_push(Stack *stack, int val);
int stack_pop(Stack *stack);
int stack_top(Stack *stack);
bool stack_is_empty(Stack *stack);
int stack_size(Stack *stack);

#endif
