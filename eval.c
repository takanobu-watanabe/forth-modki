#include "eval.h"

#include <string.h>
#include <assert.h>

void eval(CharSource *src, Stack *st) {
    Token out_token;

    while (parse_one(src, &out_token)) {
        switch (out_token.type) {
            case TOKEN_INT:
                stack_push(st, element_int(out_token.u.ival));
                break;
            case TOKEN_EXEC_NAME:
                if (strcmp(out_token.u.name, "add")==0) {
                    Element val = stack_pop(st);
                    Element val2 = stack_pop(st);
                    // add は整数どうしにしか使えない
                    assert(val.type == ELEM_INT && val2.type == ELEM_INT);
                    stack_push(st, element_int(val.u.ival + val2.u.ival));
                }
                break;
            default: 
                break;
       }
    }   
}
