#ifndef ELEMENT_H
#define ELEMENT_H

#define ELEMENT_NAME_MAX_SIZE 64

// 前方宣言。Stack* を受け取り void を返す関数へのポインタ型。
// プリミティブ（add など組み込み演算）はスタックを直接操作する。
typedef struct Stack Stack;
typedef void (*PrimitiveFn)(Stack *st);

// スタックや辞書に積める値の種類。
// 08章で ELEM_EXEC_ARRAY を追加する予定。
typedef enum {
    ELEM_INT,
    ELEM_LITERAL_NAME,
    ELEM_PRIMITIVE,
} ElementType;

// タグ付きunion。type を見て、対応するメンバだけを読み書きする。
typedef struct {
    ElementType type;
    union {
        int ival;                            // ELEM_INT のとき有効
        char name[ELEMENT_NAME_MAX_SIZE];    // ELEM_LITERAL_NAME のとき有効
        PrimitiveFn fn;                      // ELEM_PRIMITIVE のとき有効
    } u;
} Element;

// 生成ヘルパー（毎回 type と u を手で埋めなくて済む）
Element element_int(int val);
Element element_literal_name(const char *name);
Element element_primitive(PrimitiveFn fn);

#endif
