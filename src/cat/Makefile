CFLAGS=-Wall -Werror -Wextra

.PHONY: s21_cat

all: s21_cat

s21_cat:
	gcc $(CFLAGS) -o s21_cat s21_cat.c

s21_cat_debug:
	gcc -g -o s21_cat s21_cat.c

s21_cat_test: s21_cat
	./tests/s21_cat_test.sh

s21_cat_leaks: s21_cat_debug
	./tests/s21_cat_leaks_test.sh

clean:
	-rm s21_cat

rebuild: clean s21_cat