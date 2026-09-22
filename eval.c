#include "eval.h"

#include <string.h>
#include <assert.h>
#include "dict.h"
#include "parser.h"
#include "exec_array.h"

// ELEM_EXEC_NAME なら辞書を引いて実行/積む、それ以外はそのまま積む。
void eval_element(Element e, Stack *st, Dict *dict) {
    if (e.type == ELEM_EXEC_NAME) {
        Element elem;

        if (dict_get(dict, e.u.name, &elem)) {
            switch (elem.type) {
                case ELEM_PRIMITIVE:
                    elem.u.fn(st);
                    break;
                case ELEM_EXEC_ARRAY:
                    eval_exec_array(elem.u.exec_array, st, dict);
                    break;
                default:
                    stack_push(st, elem);
                    break;
            }
        } else {
            fprintf(stderr, "unknown word: %s\n", e.u.name);
        }
    } else {
        stack_push(st, e);
    }
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
                if (strcmp(out_token.u.name, "def")==0) {
                    Element value = stack_pop(st); 
                    Element name = stack_pop(st); 
                    assert(name.type == ELEM_LITERAL_NAME);
                    //assert(value.type == ELEM_INT);
                    dict_put(dict, name.u.name, value);
                } 
                else {
                    Element elem;
                    if (dict_get(dict, out_token.u.name, &elem)) {
                        switch (elem.type) {
                            case ELEM_PRIMITIVE:
                                elem.u.fn(st);
                                break;
                            case ELEM_EXEC_ARRAY:
                                eval_exec_array(elem.u.exec_array, st, dict);
                                break;
                            default:
                                stack_push(st, elem);
                                break;
                        }
                    } else {
                        fprintf(stderr, "unknown word: %s\n", out_token.u.name);
                    }
                }
                break;
            default:
                break;
        }
    }
}
