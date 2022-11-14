#ifndef _SRC_GREP_S21_GREP_H_
#define _SRC_GREP_S21_GREP_H_
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pcre.h>

void extend_pattern(char *old, char *add);

char *get_pattern(int argc, char **argv, int *pcre_opts);

#endif  // _SRC_GREP_S21_GREP_H_