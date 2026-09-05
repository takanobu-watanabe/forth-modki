#include "parser.h"
#include <limits.h>

int  cl_getc(CharSource *src) {
    return fgetc(src->fp);
}

void cl_ungetc(CharSource *src, int c){
    ungetc(c, src->fp);
}

static bool is_space(int c) {
    return c == ' ' || c == '\n' || c == '\t';
}

static bool is_number(int c) {
    return (c >= '0' && c <= '9');
}

bool parse_int(CharSource *src, int *out) {
    int c;
    int val = 0;
    bool has_digit = false;

    while((c = cl_getc(src)) != EOF) {
        if (is_space(c)){
            if (has_digit) {
                break;
            } else {
                continue;
            }
        }

        if (is_number(c)) {
            int digit = c - '0';
            if (val > (INT_MAX - digit) / 10) {
                return false;
            }
            val = val * 10 + (c - '0');
            has_digit = true;
        } else {
            cl_ungetc(src, c);
            break;
        }
    }
    if (has_digit) {
        *out = val;
        return true;
    } else {
        return false;
    }
}