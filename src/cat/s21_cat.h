#ifndef _SRC_CAT_S21_CAT_H_
#define _SRC_CAT_S21_CAT_H_

#include <stdio.h>
#include <string.h>

#define SHOW_UNPRINTABLES "v"

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

typedef void (*interpreter)(char *, char, char);
typedef void (*post_interpreter)(char *, char, size_t *);
typedef char (*line_counter)(size_t *, char, char);

char select_option(char *option, interpreter *inter,
                   post_interpreter *post_inter, line_counter *line_c);

void interpret_nonprint(char *out_str, char symb, char twice);
void passthrough(char *out_str, char c, char twice);

char non_blank_line_counter(size_t *counter, char p_ch, char c_ch);
char any_line_counter(size_t *counter, char p_ch, char c_ch);
char end_counter(size_t *counter, char p_ch, char c_ch);
char no_counter(size_t *counter, char p_ch, char c_ch);
char tabs_counter(size_t *counter, char p_ch, char c_ch);

void number(char *out, char changed, size_t *count);
void squeeze_blank(char *out, char changed, size_t *count);
void mark_lines_end(char *out, char changed, size_t *count);
void post_passthrough(char *out, char changed, size_t *count);
void mark_tabs(char *out, char changed, size_t *count);

void printer(FILE *file, interpreter inter, post_interpreter post_inter,
             line_counter line_c);
#endif  // _SRC_CAT_S21_CAT_H_
