CFLAGS=-Wall -Werror -Wextra -g
EXECUTABLES=s21_cat

.PHONY: $(EXECUTABLES)

all: $(EXECUTABLES)

s21_cat:
	gcc $(CFLAGS) -o cat/s21_cat cat/s21_cat.c

clean: rm cat/s21_cat

rebuild: clean s21_cat
