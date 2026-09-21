#include "eval.h"

#include <string.h>

void eval(CharSource *src, Stack *st) {
    Token out_token;

    while (parse_one(src, &out_token)) {
        switch (out_token.type) {
            case TOKEN_INT:
                stack_push(st, out_token.u.ival);
                break;
            case TOKEN_EXEC_NAME:
                if (strcmp(out_token.u.name, "add")==0) {
                    int val = stack_pop(st);
                    int val2 = stack_pop(st);
                    stack_push(st, val+val2);
                }
                break;
            default: 
                break;
       }
    }   
}