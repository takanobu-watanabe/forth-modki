#include "eval.h"

#include <string.h>
#include <assert.h>
#include "dict.h"
#include "parser.h"

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
            case TOKEN_OPEN_BRACE:
                break;
            case TOKEN_EXEC_NAME:
                if (strcmp(out_token.u.name, "def")==0) {
                    Element value = stack_pop(st); 
                    Element name = stack_pop(st); 
                    assert(name.type == ELEM_LITERAL_NAME);
                    assert(value.type == ELEM_INT);
                    dict_put(dict, name.u.name, value);
                } else {
                    Element elem;

                    if (dict_get(dict, out_token.u.name, &elem)) {
                        if (elem.type == ELEM_PRIMITIVE) {
                            elem.u.fn(st);
                        } else {
                            stack_push(st, elem);
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
