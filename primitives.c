#include "primitives.h"

#include "stack.h"
#include "element.h"
#include "exec_array.h"   // print で ExecArray の count を読むため
#include <assert.h>
#include <stdio.h>
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

void prim_mod(Stack *st, Dict *dict) {
    (void)dict;
    Element b = stack_pop(st);
    Element a = stack_pop(st);
    assert(b.type == ELEM_INT);
    assert(a.type == ELEM_INT);
    stack_push(st, element_int(a.u.ival % b.u.ival));
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

// /name 値 def : 名前と値を辞書に登録する
// 05章では eval の中で strcmp による特別扱いだったが、09章で PrimitiveFn が
// Dict* を受け取れるようになったのでプリミティブにできる。
// 辞書に入れることで { } の中からも使えるようになる。
void prim_def(Stack *st, Dict *dict) {
    Element value = stack_pop(st);
    Element name = stack_pop(st);

    assert(name.type == ELEM_LITERAL_NAME);
    dict_put(dict, name.u.name, value);
}

// n index : てっぺんから n 個下の値をコピーして積む
//   [ a b c ] 0 index → [ a b c c ]   （dup と同じ）
//   [ a b c ] 1 index → [ a b c b ]
// ローカル変数が無いので、スタック上の「位置」で値を指すための道具。
void prim_index(Stack *st, Dict *dict) {
    (void)dict;
    Element n = stack_pop(st);

    assert(n.type == ELEM_INT);
    stack_push(st, stack_index(st, n.u.ival));
}

// { 条件 } { 本体 } while
// 条件を実行して結果を取り出し、真である限り本体を繰り返す。
// if が「1回だけ実行するか決める」のに対し、while は毎回決める。
void prim_while(Stack *st, Dict *dict) {
    Element body = stack_pop(st);
    Element cond = stack_pop(st);

    assert(body.type == ELEM_EXEC_ARRAY);
    assert(cond.type == ELEM_EXEC_ARRAY);

    for (;;) {
        Element result;

        eval_exec_array(cond.u.exec_array, st, dict);
        result = stack_pop(st);
        assert(result.type == ELEM_INT);

        if (!result.u.ival) {
            break;
        }
        eval_exec_array(body.u.exec_array, st, dict);
    }
}

// print : 値を1つ取り出して表示する
// default: を書かず5種類すべてを列挙しているので、将来 ElementType に
// 値を足したとき -Wswitch が対応漏れを教えてくれる。
void prim_print(Stack *st, Dict *dict) {
    (void)dict;
    Element e = stack_pop(st);

    switch (e.type) {
    case ELEM_INT:
        printf("%d\n", e.u.ival);
        break;
    case ELEM_LITERAL_NAME:
        printf("/%s\n", e.u.name);
        break;
    case ELEM_EXEC_NAME:
        printf("%s\n", e.u.name);
        break;
    case ELEM_EXEC_ARRAY:
        printf("{ ... }（%d要素）\n", e.u.exec_array->count);
        break;
    case ELEM_PRIMITIVE:
        printf("<組み込み関数>\n");
        break;
    }
}


void register_primitives(Dict *dict) {
    dict_put(dict, "add", element_primitive(prim_add));
    dict_put(dict, "sub", element_primitive(prim_sub));
    dict_put(dict, "mul", element_primitive(prim_mul));
    dict_put(dict, "div", element_primitive(prim_div));
    dict_put(dict, "mod", element_primitive(prim_mod));
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
    dict_put(dict, "index", element_primitive(prim_index));
    dict_put(dict, "while", element_primitive(prim_while));
    dict_put(dict, "def", element_primitive(prim_def));
    dict_put(dict, "print", element_primitive(prim_print));
}
