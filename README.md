# forth_modoki

C言語で書いた、PostScript風スタック指向言語のインタプリタです。

[karino2氏の「暇つぶしプログラム教室 C言語編」](https://karino2.github.io/c-lesson/) に沿って、
外部ライブラリを使わず標準ライブラリだけで一から実装しています。学習用のプロジェクトです。

```
$ echo "/double { 2 mul } def 5 double" > sample.ps
$ ./forth_modoki sample.ps
result: 10
```

## 現在の状況

第一部（全13章）のうち **09章まで実装済み**。変数定義・関数定義・条件分岐が動きます。

## ビルドと実行

```bash
make              # unittest / forth_modoki / trace をビルド
make test         # ユニットテストを実行
make test-asan    # AddressSanitizer 付きでテストを実行
make trace-run    # 内部動作のトレースを表示
make clean
```

`clang` または `gcc`（C11対応）だけで動きます。依存ライブラリはありません。

```bash
./forth_modoki <file.ps>        # ファイルを実行し、スタックのトップを表示
./trace "1 2 add"               # 任意のコードの内部動作を表示
```

## 使える機能

### 値

```
42          整数
-5          負の整数
/x          リテラル名（実行されず、名前そのものが積まれる）
{ 2 mul }   実行可能配列（実行されず、コードの塊として積まれる）
```

### 演算子

| 分類 | 演算子 |
|---|---|
| 算術 | `add` `sub` `mul` `div` |
| 比較 | `eq` `ne` `lt` `gt` `le` `ge` |
| スタック操作 | `dup` `pop` `exch` |
| 制御 | `if` `ifelse` |
| 定義 | `def` |

真偽値は専用の型を持たず、`0` を偽、`0` 以外を真として整数で表します。

### 書ける例

```
1 2 add                              → 3
10 3 sub                             → 7
/x 5 def  x x add                    → 10     変数
/double { 2 mul } def  5 double      → 10     関数
1 { 10 } { 20 } ifelse               → 10     条件分岐
-5 dup 0 lt { -1 mul } if            → 5      絶対値
```

## 構成

```
        main.c / test_main.c / trace.c     ← main を持つ3つの入口
                      │
                    eval          式の評価
                   ╱    ╲
          exec_array     dict     { } のコンパイル / 名前の辞書
              │           │
            parser      element   字句解析 / 値の表現
                          │
                        stack     値の置き場
```

| ファイル | 役割 |
|---|---|
| `parser.c/.h` | 文字列 → トークン列（`parse_int` / `parse_one`） |
| `element.c/.h` | スタックや辞書に入る値の表現（タグ付きunion） |
| `stack.c/.h` | オペランドスタック |
| `dict.c/.h` | 名前 → 値の辞書（ハッシュテーブル + チェイン法） |
| `exec_array.c/.h` | `{ }` のコンパイルと保持（flexible array member） |
| `eval.c/.h` | トークン列・実行可能配列の評価 |
| `primitives.c/.h` | 組み込み演算子の実装と辞書への登録 |
| `main.c` | `.ps` ファイルを実行するコマンド |
| `test_main.c` | ユニットテスト（49本） |
| `trace.c` | 内部動作を表示する学習用ツール |

`Stack` と `Dict` は構造体の中身をヘッダに公開せず、`.c` の中に隠しています
（不完全型 + ポインタ）。おかげで辞書を配列＋線形探索からハッシュテーブルへ
差し替えたとき、利用側は1行も変更せずに済みました。

## テスト

```bash
make test        # 49本のテスト
make test-asan   # 同じテストを AddressSanitizer 付きで
```

`test-asan` を用意しているのは、`realloc` による配列の拡張を止めても
通常のテストが通ってしまった経験があるためです。領域外書き込みは未定義動作で、
たまたま動いてしまうことがあります。`malloc` / `realloc` まわりを触ったときは
こちらも実行しています。

## トレース

内部で何が起きているかを表示するツールを同梱しています。

```bash
$ ./trace "/double { 2 mul } def 5 double"
```

```
[1] トークン: /double  (LITERAL_NAME)
    名前そのものを積む（実行しない）
    スタック: [ LITERAL_NAME "double" ]

[2] トークン: {  (OPEN_BRACE)
    compile_exec_array で } まで読み取る（実行はしない）
    できたもの: EXEC_ARRAY(count=2)
    items[0] = INT 2
    items[1] = EXEC_NAME "mul"
    それを1個の値として積む
    スタック: [ LITERAL_NAME "double" ][ EXEC_ARRAY(count=2) ]

[3] トークン: def  (EXEC_NAME)
    def は特別扱い。2つ pop する
        値   = EXEC_ARRAY(count=2)
        名前 = LITERAL_NAME "double"
    辞書に登録: "double" → EXEC_ARRAY(count=2)
    スタック: [空]

...
```

## これから

| 章 | 内容 |
|---|---|
| 10 | 階乗・FizzBuzz・GCD をこの言語で書く |
| 11 | 継続スタックを導入し、Cの呼び出しスタックへの依存をなくす |
| 12 | `ifelse` をジャンプ命令で再実装する |
| 13 | ループとローカル変数 |

11〜13章は内部実装の大きな書き換えになります。安全網は既存のテストです。

## 既知の制限

- 整数は `int` の範囲のみ。`INT_MIN` は読めない（`parse_int` が非負しか扱わないため）
- 文字列型・浮動小数点数は未対応
- `exec_array_free` は入れ子の実行可能配列を解放しない
- エラーは `stderr` への出力か `assert` による停止で、実行時エラーとして扱う仕組みはない

## 出典

- [暇つぶしプログラム教室 C言語編](https://karino2.github.io/c-lesson/)（karino2氏）
