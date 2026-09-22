#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define NAME_MAX_SIZE 64

typedef struct {
    FILE *fp;
} CharSource;

typedef enum {
    TOKEN_INT,
    TOKEN_EXEC_NAME,
    TOKEN_LITERAL_NAME,
    TOKEN_OPEN_BRACE,
    TOKEN_CLOSE_BRACE,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    union {
        int ival;
        char name[NAME_MAX_SIZE];
    } u;
} Token;

int  cl_getc(CharSource *src);
void cl_ungetc(CharSource *src, int c);

// 非負整数を1つ読む（符号には対応しない）。
// 戻り値: 数字を1つ以上読み、int の範囲に収まれば true。
// 数字を読めなかった場合（EOF等）や桁あふれの場合は false。
// 成功時だけ *out に値を書き込む。失敗しても消費した入力は巻き戻さない。
bool parse_int(CharSource *src, int *out);

bool parse_one(CharSource *src, Token *out_token);

void parser_print_all(CharSource *src);
#endif
