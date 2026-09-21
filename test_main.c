#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "test_util.h"

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

    if (g_test_fail_count == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("%d TEST(S) FAILED\n", g_test_fail_count);
        return 1;
    }
}
