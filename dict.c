#include "dict.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUCKET_COUNT 64
#define DICT_NAME_MAX_SIZE 64

// 同じバケットに落ちたエントリを連結リストでつなぐ（チェイン法）
typedef struct EntryNode {
    char name[DICT_NAME_MAX_SIZE];
    int value;
    struct EntryNode *next;
} EntryNode;

struct Dict {
    EntryNode *buckets[BUCKET_COUNT];
};

// djb2 の簡易版。unsigned を使うのは、桁あふれの挙動が規格で
// 定義されている（2^32 で割った余りになる）ため。
static unsigned int hash_str(const char *s) {
    unsigned int h = 5381;

    while (*s) {
        h = h * 33 + (unsigned char)(*s);
        s++;
    }
    return h;
}

static unsigned int bucket_index(const char *name) {
    return hash_str(name) % BUCKET_COUNT;
}

Dict *dict_new(void) {
    Dict *dict = malloc(sizeof(Dict));

    assert(dict != NULL);
    memset(dict->buckets, 0, sizeof(dict->buckets));
    return dict;
}

void dict_free(Dict *dict) {
    for (int i = 0; i < BUCKET_COUNT; i++) {
        EntryNode *node = dict->buckets[i];

        while (node != NULL) {
            // free する前に next を退避する。順番を逆にすると
            // 解放済みメモリを読むことになる。
            EntryNode *next = node->next;

            free(node);
            node = next;
        }
    }
    free(dict);
}

void dict_put(Dict *dict, const char *name, int value) {
    unsigned int h = bucket_index(name);
    EntryNode *node;

    assert(strlen(name) < DICT_NAME_MAX_SIZE);

    for (node = dict->buckets[h]; node != NULL; node = node->next) {
        if (strcmp(node->name, name) == 0) {
            node->value = value;
            return;
        }
    }

    // 見つからなかったのでリストの先頭に挿入する（末尾を探す必要がない）
    node = malloc(sizeof(EntryNode));
    assert(node != NULL);
    strcpy(node->name, name);
    node->value = value;
    node->next = dict->buckets[h];
    dict->buckets[h] = node;
}

bool dict_get(Dict *dict, const char *name, int *out_value) {
    unsigned int h = bucket_index(name);

    for (EntryNode *node = dict->buckets[h]; node != NULL; node = node->next) {
        if (strcmp(node->name, name) == 0) {
            *out_value = node->value;
            return true;
        }
    }
    return false;
}
