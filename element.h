#ifndef ELEMENT_H
#define ELEMENT_H

#define ELEMENT_NAME_MAX_SIZE 64

// スタックに積める値の種類。
// 07章で ELEM_C_FUNC、08章で ELEM_EXEC_ARRAY を追加する予定。
typedef enum {
    ELEM_INT,
    ELEM_LITERAL_NAME,
} ElementType;

// タグ付きunion。type を見て、対応するメンバだけを読み書きする。
typedef struct {
    ElementType type;
    union {
        int ival;                            // ELEM_INT のとき有効
        char name[ELEMENT_NAME_MAX_SIZE];    // ELEM_LITERAL_NAME のとき有効
    } u;
} Element;

// 生成ヘルパー（毎回 type と u を手で埋めなくて済む）
Element element_int(int val);
Element element_literal_name(const char *name);

#endif
