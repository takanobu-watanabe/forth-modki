#include "dict.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DICT_MAX_SIZE 128
#define DICT_NAME_MAX_SIZE 64

// 1件の登録を表す
typedef struct {
    char name[DICT_NAME_MAX_SIZE];
    int value;
} Entry;

// 辞書の本体
struct Dict {
    Entry entries[DICT_MAX_SIZE];
    int count;   // 現在の登録件数
};

Dict *dict_new(void) {
    Dict *dict = malloc(sizeof(Dict));
    assert(dict != NULL);
    dict->count = 0;
    return dict;
}

void dict_free(Dict *dict) {
    free(dict);
}

// name に value を登録する。同じ name が既にあれば上書きする。
void dict_put(Dict *dict, const char *name, int value) {
    assert(dict->count < DICT_MAX_SIZE);
    for (int i = 0; i < dict->count; i++) {
        if (strcmp(dict->entries[i].name, name)==0) {
            dict->entries[i].value = value;
            return;
        }
    }
    strcpy(dict->entries[dict->count].name, name);
    dict->entries[dict->count].value = value;
    dict->count += 1;
}


// name を引く。見つかれば *out_value に書き込んで true、無ければ false。
bool dict_get(Dict *dict, const char *name, int *out_value) {
    for (int i = 0; i < dict->count; i++) {
        if (strcmp(dict->entries[i].name, name)==0) {
            *out_value = dict->entries[i].value;
            return true;
        }
    }
    return false;
}