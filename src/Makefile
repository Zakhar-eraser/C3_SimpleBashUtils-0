CFLAGS=-Wall -Werror -Wextra
EXECUTABLES=s21_cat s21_grep

.PHONY: $(EXECUTABLES)

all: $(EXECUTABLES)

s21_cat:
	gcc $(CFLAGS) -o cat/s21_cat cat/s21_cat.c

s21_cat_debug:
	gcc $(CFLAGS) -g -o cat/s21_cat cat/s21_cat.c

s21_grep:
	gcc $(CFLAGS) -o grep/s21_grep grep/s21_grep.c -lpcre

s21_grep_test: s21_grep
	./tests/s21_grep_test.sh

s21_grep_debug:
	gcc $(CFLAGS) -g -o grep/s21_grep grep/s21_grep.c -lpcre

clean: rm cat/s21_cat grep/s21_grep

rebuild: clean s21_cat s21_grep
