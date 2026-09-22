#include "primitives.h"

#include "stack.h"
#include "element.h"
#include <assert.h>
#include "eval.h"

void prim_add(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival + b.u.ival));
}

void prim_sub(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival - b.u.ival));
}

void prim_mul(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival * b.u.ival));
}

void prim_div(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival / b.u.ival));
}

void prim_dup(Stack *st, Dict *dict) {
    (void)dict;
    stack_dup(st);
}

void prim_pop(Stack *st, Dict *dict) {
    (void)dict;
    stack_pop(st);
}

void prim_eq(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival == b.u.ival));
}

void prim_ne(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival != b.u.ival));
}

void prim_lt(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival < b.u.ival));
}

void prim_gt(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival > b.u.ival));
}

void prim_le(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival <= b.u.ival));
}

void prim_ge(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival >= b.u.ival));
}

void prim_exch(Stack *st, Dict *dict) {
    (void)dict;
    stack_exch(st);
}

void prim_if(Stack *st, Dict *dict) {
    Element proc = stack_pop(st);
    Element cond = stack_pop(st);

    assert(proc.type == ELEM_EXEC_ARRAY);
    assert(cond.type == ELEM_INT);

    if (cond.u.ival) {
        eval_exec_array(proc.u.exec_array, st, dict);
    }
}

//"1 { 10 } { 20 } ifelse
void prim_ifelse(Stack *st, Dict *dict) {
    Element proc2 = stack_pop(st);   // 偽のとき用（後に積まれたので先に出る）
    Element proc1 = stack_pop(st);   // 真のとき用
    Element cond  = stack_pop(st);

    assert(proc2.type == ELEM_EXEC_ARRAY);
    assert(proc1.type == ELEM_EXEC_ARRAY);
    assert(cond.type == ELEM_INT);
    // 3つとも型を assert

    if (cond.u.ival == 1) {
        eval_exec_array(proc1.u.exec_array, st, dict);
    } else {
        eval_exec_array(proc2.u.exec_array, st, dict);
    }
    // cond が真なら proc1、偽なら proc2 を eval_exec_array で実行
}


void register_primitives(Dict *dict) {
    dict_put(dict, "add", element_primitive(prim_add));
    dict_put(dict, "sub", element_primitive(prim_sub));
    dict_put(dict, "mul", element_primitive(prim_mul));
    dict_put(dict, "div", element_primitive(prim_div));
    dict_put(dict, "dup", element_primitive(prim_dup));
    dict_put(dict, "pop", element_primitive(prim_pop));
    dict_put(dict, "eq", element_primitive(prim_eq));
    dict_put(dict, "ne", element_primitive(prim_ne));
    dict_put(dict, "lt", element_primitive(prim_lt));
    dict_put(dict, "gt", element_primitive(prim_gt));
    dict_put(dict, "le", element_primitive(prim_le));
    dict_put(dict, "ge", element_primitive(prim_ge));
    dict_put(dict, "exch", element_primitive(prim_exch));
    dict_put(dict, "if", element_primitive(prim_if));
    dict_put(dict, "ifelse", element_primitive(prim_ifelse));

}
