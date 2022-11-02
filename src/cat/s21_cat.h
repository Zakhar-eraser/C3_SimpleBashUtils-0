#ifndef _SRC_CAT_S21_CAT_H_
#define _SRC_CAT_S21_CAT_H_

#include <string.h>
#include <stdio.h>
#include "common/utils.h"

#define TAB "^I"
#define NL "^J"

#define NUMBER_NONBLANK "b"
#define NUMBER_NONBLANK_GNU "-number-nonblank"

#define SHOW_ENDS "e"
#define SHOW_ENDS_GNU "E"

#define NUMBER "n"
#define NUMBER_GNU "-number"

#define SQUEEZE_BLANK "s"
#define SQUEEZE_BLANK_GNU "-squeeze-blank"

#define SHOW_TABS "t"
#define SHOW_TABS_GNU "T"

typedef void (*interpreter)(char*, char);
typedef void(*post_interpreter)(char *, char, size_t *);
typedef void (*line_counter)(size_t *, char, char);

void interpret_nonprint(char *out_str, char symb);
void passthrough(char *out_str, char c);

void squeeze_line_counter(size_t *counter, char p_ch, char c_ch);
void non_blank_line_counter(size_t *counter, char p_ch, char c_ch);
void any_line_counter(size_t *counter, char p_ch, char c_ch);

void number(char *out, char c_ch, size_t *count);
void squeeze_blank(char *out, char c_ch, size_t *count);
void mark_lines_end(char *out, char c_ch, size_t *count);

void printer(
    FILE *file,
    interpreter inter,
    post_interpreter post_inter,
    line_counter line_c);
#endif  // _SRC_CAT_S21_CAT_H_
