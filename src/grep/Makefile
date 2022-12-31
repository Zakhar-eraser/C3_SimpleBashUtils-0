CFLAGS=-Wall -Werror -Wextra

.PHONY: s21_grep

all: s21_grep

s21_grep:
	gcc $(CFLAGS) -o s21_grep s21_grep.c -lpcre

s21_grep_test: s21_grep
	./tests/s21_grep_test.sh

s21_grep_debug:
	gcc -g -o s21_grep s21_grep.c -lpcre

s21_grep_leaks: s21_grep_debug
	./tests/s21_grep_leaks_test.sh

clean:
	rm s21_grep

rebuild: clean s21_grep