#include "cont.h"

#include <stdlib.h>
#include <assert.h>

#define CONTSTACK_MAX_SIZE 256

struct ContStack {
    Continuation frames[CONTSTACK_MAX_SIZE];
    int top;   // 次に積む位置。0 なら空（Stack と同じ約束）
};

ContStack* contstack_new(void) {
    ContStack *cs = malloc(sizeof(ContStack));
    assert(cs != NULL); 
    cs->top = 0;
    return cs;
}

void contstack_free(ContStack *cs) {
    free(cs);
}

void contstack_push(ContStack *cs, ExecArray *ea) {
    // 満杯でないことを assert してから、ea と pc=0 を積む
    assert(cs->top < CONTSTACK_MAX_SIZE);
    cs->frames[cs->top].ea = ea;
    cs->frames[cs->top].pc = 0;
    cs->top++;
}

Continuation *contstack_top(ContStack *cs) {
    // 空でないことを assert してから、てっぺんのアドレスを返す
    assert(cs->top != 0);
    return &cs->frames[cs->top-1];
}

void contstack_pop(ContStack *cs) {
    // 空でないことを assert してから、top を1つ減らす
    assert(cs->top != 0);
    cs->top--;
}

bool contstack_is_empty(ContStack *cs) {
    if (cs->top == 0) {
        return true;
    } else {
        return false;
    }
}

int contstack_size(ContStack *cs) {
    return cs->top;
}
