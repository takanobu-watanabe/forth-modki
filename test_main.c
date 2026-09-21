#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "test_util.h"
#include "stack.h"
#include "eval.h"
#include "dict.h"

// Element が ELEM_INT で、期待した値を持つことを検証する
#define UT_EQ_ELEM_INT(expected, actual) do { \
    Element _el = (actual); \
    UT_EQ_INT(ELEM_INT, _el.type); \
    if (_el.type == ELEM_INT) { \
        UT_EQ_INT((expected), _el.u.ival); \
    } \
} while (0)

// 文字列を入力にした CharSource を作るヘルパー
CharSource make_src_from_string(const char *str) {
    CharSource src;

    src.fp = fmemopen((void*)str, strlen(str), "r");
    return src;
}

// 演習1-1: "123" を渡して 123 が読めることを確認する
void test_parse_int_single(void) {
    int num;
    bool b;

    CharSource src = make_src_from_string("123");
    b = parse_int(&src, &num);
    UT_TRUE(b);
    if (b){
        UT_EQ_INT(123, num);
    }
    fclose(src.fp);
}

// 演習1-1: 前後に空白がある "  42  " でも読めることを確認する
void test_parse_int_with_spaces(void) {
    int num;
    bool b;

    CharSource src = make_src_from_string("  42  ");
    b = parse_int(&src, &num);
    UT_TRUE(b);
    if (b){
        UT_EQ_INT(42, num);
    }
    fclose(src.fp);
}

// 演習1-2: "1 2 3" から3回呼んで 1, 2, 3 を順に取り出せることを確認する
void test_parse_int_multiple(void) {
    CharSource src = make_src_from_string("1 2 3");
    int num;
    bool b;
  
    b = parse_int(&src, &num);
    UT_TRUE(b);
    if (b){
        UT_EQ_INT(1, num);
    }
    b = parse_int(&src, &num);
    UT_TRUE(b);
    if (b){
        UT_EQ_INT(2, num);
    }
    b = parse_int(&src, &num);
    UT_TRUE(b);
    if (b){
        UT_EQ_INT(3, num);
    }
    fclose(src.fp);
}

// 演習1-2: 空文字列 "" では false が返ることを確認する
void test_parse_int_empty(void) {
    CharSource src = { .fp = tmpfile() };
    UT_TRUE(src.fp != NULL);
    if (src.fp == NULL) {
        return;
    }

    int num = 0;
    bool b = parse_int(&src, &num);
    UT_TRUE(!b);

    fclose(src.fp);
}

// 演習1-3（発展）: "abc" では false が返り、'a' が ungetc で正しく戻っていることを確認する
void test_parse_int_non_digit(void) { 
    CharSource src = make_src_from_string("abc");
    int num;
    bool b;
  
    b = parse_int(&src, &num);
    UT_TRUE(!b);
    UT_EQ_INT('a', cl_getc(&src));    // 戻された 'a' を読める
    fclose(src.fp);
}

// int の最大値までは読み取れることを確認する
void test_parse_int_max(void) {
    char input[sizeof(int) * CHAR_BIT + 2];
    snprintf(input, sizeof(input), "%d", INT_MAX);
    CharSource src = make_src_from_string(input);
    UT_TRUE(src.fp != NULL);
    if (src.fp == NULL) {
        return;
    }

    int num = 0;
    bool b = parse_int(&src, &num);
    UT_TRUE(b);
    if (b) {
        UT_EQ_INT(INT_MAX, num);
    }
    fclose(src.fp);
}

// int の最大値を1超える入力では false が返ることを確認する
void test_parse_int_overflow(void) {
    char input[sizeof(int) * CHAR_BIT + 2];
    // テスト用の数値を作る計算自体がオーバーフローしないよう unsigned を使う
    snprintf(input, sizeof(input), "%u", (unsigned int)INT_MAX + 1U);
    CharSource src = make_src_from_string(input);
    UT_TRUE(src.fp != NULL);
    if (src.fp == NULL) {
        return;
    }

    int num = 0;
    bool b = parse_int(&src, &num);
    UT_TRUE(!b);
    fclose(src.fp);
}

void test_parse_one_int(void){
    CharSource src = make_src_from_string("42");
    Token t = { .type = TOKEN_EOF };

    bool b = parse_one(&src, &t);
    UT_TRUE(b);
    if (b){
        UT_EQ_INT(TOKEN_INT, t.type);
        if (t.type == TOKEN_INT) {
            UT_EQ_INT(42, t.u.ival);
        }
    }
    fclose(src.fp);
}

void test_parse_one_exec_name(void) {
    CharSource src = make_src_from_string("add");
    Token t = { .type = TOKEN_EOF };

    bool b = parse_one(&src, &t);
    UT_TRUE(b);
    if (b){
        UT_EQ_INT(TOKEN_EXEC_NAME, t.type);
        if (t.type == TOKEN_EXEC_NAME) {
            UT_EQ_INT(0, strcmp("add", t.u.name));
        }
    }
    fclose(src.fp);
}

void test_parse_one_literal_name(void) {
    CharSource src = make_src_from_string("/double");
    Token t = { .type = TOKEN_EOF };

    bool b = parse_one(&src, &t);
    UT_TRUE(b);
    if (b){
        UT_EQ_INT(TOKEN_LITERAL_NAME, t.type);
        if (t.type == TOKEN_LITERAL_NAME) {
            UT_EQ_INT(0, strcmp("double", t.u.name));
        }
    }
    fclose(src.fp);
}

void test_parse_one_open_brace(void) {
    CharSource src = make_src_from_string("{");
    Token t = { .type = TOKEN_EOF };

    bool b = parse_one(&src, &t);
    UT_TRUE(b);
    if (b){
        UT_EQ_INT(TOKEN_OPEN_BRACE, t.type);
    }
    fclose(src.fp);
}

void test_parse_one_close_brace(void) {
    CharSource src = make_src_from_string("}");
    Token t = { .type = TOKEN_EOF };

    bool b = parse_one(&src, &t);
    UT_TRUE(b);
    if (b){
        UT_EQ_INT(TOKEN_CLOSE_BRACE, t.type);
    }
    fclose(src.fp);
}

void test_stack_push_pop(void) {
    Stack *st = stack_new(10);
    stack_push(st, element_int(1));
    stack_push(st, element_int(2));
    
    UT_EQ_ELEM_INT(2, stack_pop(st));

    UT_EQ_ELEM_INT(1, stack_pop(st));

    UT_TRUE(stack_is_empty(st));

    
    stack_free(st);
}

void test_stack_top_does_not_pop(void) {
    Stack *st = stack_new(10);

    stack_push(st, element_int(5));
    UT_EQ_ELEM_INT(5, stack_top(st));
    UT_EQ_INT(1, stack_size(st));
    stack_free(st);
}

void test_stack_dup(void) {
    Stack *st = stack_new(10);

    stack_push(st, element_int(5));
    stack_dup(st);
    UT_EQ_INT(2, stack_size(st));  
    UT_EQ_ELEM_INT(5, stack_pop(st));
    UT_EQ_ELEM_INT(5, stack_pop(st));
    UT_TRUE(stack_is_empty(st));
  
    
    stack_free(st);
}

void test_stack_dup_keeps_lower(void) {
    Stack *st = stack_new(10);

    stack_push(st, element_int(1));
    stack_push(st, element_int(2));
    stack_dup(st);

    UT_EQ_INT(3, stack_size(st));
    
    UT_EQ_ELEM_INT(2, stack_pop(st));
    UT_EQ_ELEM_INT(2, stack_pop(st));
    UT_EQ_ELEM_INT(1, stack_pop(st));

    stack_free(st);
}

void test_stack_exch(void) {
    Stack *st = stack_new(10);

    stack_push(st, element_int(1));
    stack_push(st, element_int(2));
    stack_exch(st);

    UT_EQ_INT(2, stack_size(st)); 
    UT_EQ_ELEM_INT(1, stack_pop(st));
    UT_EQ_ELEM_INT(2, stack_pop(st));
    stack_free(st);

}


void test_eval_push_only(void) {
    CharSource src = make_src_from_string("1 2 3");
    Stack *st = stack_new(10);
    eval(&src, st);
    // size が 3、pop が 3 → 2 → 1 の順
    UT_EQ_INT(3, stack_size(st)); 
    UT_EQ_ELEM_INT(3, stack_pop(st));
    UT_EQ_ELEM_INT(2, stack_pop(st));
    UT_EQ_ELEM_INT(1, stack_pop(st));
    stack_free(st);
}

void test_eval_add(void) {
    CharSource src = make_src_from_string("1 2 add");
    Stack *st = stack_new(10);
    eval(&src, st);
    UT_EQ_INT(1, stack_size(st)); 
    UT_EQ_ELEM_INT(3, stack_pop(st));
    stack_free(st);
}

void test_eval_add_twice(void) {
    CharSource src = make_src_from_string("1 2 add 10 add");
    Stack *st = stack_new(10);
    eval(&src, st);
    UT_EQ_INT(1, stack_size(st)); 
    UT_EQ_ELEM_INT(13, stack_pop(st));
    stack_free(st);
}

void test_dict_put_get(void) {
    Dict *dict = dict_new();
    int val;
    bool found;

    dict_put(dict, "x", 5);

    found = dict_get(dict, "x", &val);
    UT_TRUE(found);
    if (found) {
        UT_EQ_INT(5, val);
    }

    dict_free(dict);
}

void test_dict_put_overwrites(void) {
    Dict *dict = dict_new();
    int val;
    bool found;

    dict_put(dict, "x", 5);
    dict_put(dict, "x", 10);

    found = dict_get(dict, "x", &val);
    UT_TRUE(found);
    if (found) {
        UT_EQ_INT(10, val);
    }

    dict_free(dict);
}

void test_dict_get_undefined_returns_false(void) {
    Dict *dict = dict_new();
    int val;
    bool found;

    found = dict_get(dict, "y", &val);
    UT_TRUE(!found);

    dict_free(dict);
}

int main(void) {
    test_parse_int_single();
    test_parse_int_with_spaces();
    test_parse_int_multiple();
    test_parse_int_empty();
    test_parse_int_non_digit();
    test_parse_int_max();
    test_parse_int_overflow();
    test_parse_one_int();
    test_parse_one_exec_name();
    test_parse_one_literal_name();
    test_parse_one_open_brace();
    test_parse_one_close_brace();
    test_stack_push_pop();
    test_stack_top_does_not_pop();
    test_stack_dup();
    test_stack_dup_keeps_lower();
    test_stack_exch();
    test_eval_push_only();
    test_eval_add();
    test_eval_add_twice();
    test_dict_put_get();
    test_dict_put_overwrites();
    test_dict_get_undefined_returns_false();

    if (g_test_fail_count == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("%d TEST(S) FAILED\n", g_test_fail_count);
        return 1;
    }
}
