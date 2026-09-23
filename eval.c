#include "eval.h"

#include <string.h>
#include <assert.h>
#include "dict.h"
#include "parser.h"
#include "exec_array.h"
#include "cont.h"

void eval_element(Element e, Stack *st, Dict *dict) {
    ContStack *cs = contstack_new();

    eval_element_vm(e, st, cs, dict);
    run(cs, st, dict);

    contstack_free(cs);
}

void eval_exec_array(ExecArray *ea, Stack *st, Dict *dict) {
    // ea->count 個の items を、先頭から順に eval_element で評価する
    for (int i = 0; i < ea->count; i++) {
        eval_element(ea->items[i], st, dict);
    }
}

void eval(CharSource *src, Stack *st, Dict *dict) {
    Token out_token;

    while (parse_one(src, &out_token)) {
        switch (out_token.type) {
            case TOKEN_INT:
                stack_push(st, element_int(out_token.u.ival));
                break;
            case TOKEN_LITERAL_NAME:
                stack_push(st, element_literal_name(out_token.u.name));
                break;
            case TOKEN_OPEN_BRACE: {
                ExecArray *ea = compile_exec_array(src);
                stack_push(st, element_exec_array(ea));
                break;
            }
            case TOKEN_EXEC_NAME:
                // 特別扱いは無い。辞書を引いて型で振り分ける処理は
                // eval_element がそのまま持っているので委ねる。
                eval_element(element_exec_name(out_token.u.name), st, dict);
                break;
            default:
                break;
        }
    }
}

void eval_element_vm(Element e, Stack *st, ContStack *cs, Dict *dict) {
    if (e.type == ELEM_EXEC_NAME) {
        Element found;

        if (dict_get(dict, e.u.name, &found)) {
            switch (found.type) {
                case ELEM_PRIMITIVE:
                    found.u.fn(st, cs, dict);
                    break;
                case ELEM_EXEC_ARRAY:
                    contstack_push(cs, found.u.exec_array);
                    break;
                default:
                    stack_push(st, found);
                    break;
            }
        } else {
            fprintf(stderr, "unknown word: %s\n", e.u.name);
        }

    } else {
        stack_push(st, e);
    }
}

void run(ContStack *cs, Stack *st, Dict *dict) {
    while (!contstack_is_empty(cs)) {
        Continuation *frame = contstack_top(cs);

        // ① 実行し終わっていたら捨てる（呼び出し元に戻る）
         if (frame->pc >= frame->ea->count) {
            contstack_pop(cs);
            continue;
        }

        // ② 今の位置の要素を取り出して、pc を進める
        Element e = frame->ea->items[frame->pc];
        frame->pc++;

        // ③ 評価する
        eval_element_vm(e, st, cs, dict);
    }
}