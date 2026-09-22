#include "stack.h"

#include <stdlib.h>
#include <assert.h>


// 中身は stack.c に隠す（使う側は Stack* しか扱えない）
struct Stack {
    Element *data; // 値を入れる配列（malloc で確保）
    int capacity;  // 配列の要素数（確保した上限）
    int top;       // 次に push する位置
};

Stack* stack_new(int capacity) {
    Stack *stack = malloc(sizeof(Stack));
    assert(stack != NULL);
    stack->data = malloc(sizeof(Element) * capacity);
    stack->capacity = capacity;
    stack->top = 0;
    return stack;
}

void stack_free(Stack *stack) {
    if (stack) {
        if (stack->data) {
            free(stack->data);
        }
        free(stack);
    }
}

void stack_push(Stack *stack, Element val) {
    assert(stack_size(stack) < stack->capacity);
    stack->data[stack->top] = val;
    stack->top += 1;
}

Element stack_top(Stack *stack) {
    assert(!stack_is_empty(stack));
    return stack->data[stack->top-1];
}

Element stack_index(Stack *stack, int n) {
    assert(n >= 0);
    assert(n < stack->top);        // n 個下が存在すること
    return stack->data[stack->top - 1 - n];
}

Element stack_pop(Stack *stack) {
    Element val;

    assert(!stack_is_empty(stack));
    stack->top -= 1;
    val = stack->data[stack->top];
    return val;
}

bool stack_is_empty(Stack *stack){
    if (stack->top == 0) {
        return true;
    } else {
        return false;
    }
}

int stack_size(Stack *stack) {
    return stack->top;
}

void stack_dup(Stack *stack) {
    Element top_val;

    top_val = stack_top(stack);
    stack_push(stack, top_val);
}

void stack_exch(Stack *stack){
    Element val = stack_pop(stack);
    Element val2 = stack_pop(stack);
    stack_push(stack, val); 
    stack_push(stack, val2); 
}

