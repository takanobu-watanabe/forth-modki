#ifndef ELEMENT_H
#define ELEMENT_H

#define ELEMENT_NAME_MAX_SIZE 64

// 前方宣言。プリミティブ（add など組み込み演算）が受け取る型。
// dict が要るのは if / ifelse のため。取り出した実行可能配列を
// 評価するとき、中の名前を辞書で引く必要がある。
typedef struct Stack Stack;
typedef struct Dict Dict;
typedef void (*PrimitiveFn)(Stack *st, Dict *dict);

// 前方宣言。中身（flexible array member を使った定義）は exec_array.h にある。
// ここではポインタとしてしか使わないので、不完全型のままでよい。
typedef struct ExecArray ExecArray;

// スタックや辞書に積める値の種類。
typedef enum {
    ELEM_INT,
    ELEM_LITERAL_NAME,   // /x : そのまま積む名前
    ELEM_PRIMITIVE,
    ELEM_EXEC_ARRAY,     // { ... } : コンパイル済みのコード列
    ELEM_EXEC_NAME,      // x（{ } の中にある）: 実行時まで辞書引きを遅らせる名前
} ElementType;

// タグ付きunion。type を見て、対応するメンバだけを読み書きする。
typedef struct {
    ElementType type;
    union {
        int ival;                            // ELEM_INT のとき有効
        char name[ELEMENT_NAME_MAX_SIZE];    // ELEM_LITERAL_NAME / ELEM_EXEC_NAME のとき有効
        PrimitiveFn fn;                      // ELEM_PRIMITIVE のとき有効
        ExecArray *exec_array;               // ELEM_EXEC_ARRAY のとき有効
    } u;
} Element;

// 生成ヘルパー（毎回 type と u を手で埋めなくて済む）
Element element_int(int val);
Element element_literal_name(const char *name);
Element element_primitive(PrimitiveFn fn);
Element element_exec_array(ExecArray *ea);
Element element_exec_name(const char *name);

#endif
