CFLAGS = -Wall -Wextra -std=c11 -g

UNITTEST = unittest
SRCS = test_main.c parser.c
HDRS = parser.h test_util.h

.PHONY: test clean

# テストを走らせる（バイナリが最新でも必ず実行される）
test: $(UNITTEST)
	./$(UNITTEST)

# テストバイナリのビルド（ソース/ヘッダが新しいときだけ再ビルド）
$(UNITTEST): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(UNITTEST) $(SRCS)

clean:
	rm -rf $(UNITTEST) test *.dSYM
