#ifndef _SRC_GREP_S21_GREP_H_
#define _SRC_GREP_S21_GREP_H_
#include <pcre.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct input_data {
  int argc;
  char **argv;
  char **filenames;
  size_t files_count;
} input_data;

typedef struct regexes {
  const char *error;
  int erroroffset;
  pcre **res;
  size_t ptrn_cnt;
  char *pattern;
} regexes;

typedef struct match_modifiers {
  int pcre_opts;
  int inversion;
  int only_matches_count;
  int print_line_number;
  int first_match;
  int hide_filenames;
  int hide_warnings;
  int all_matches;
} match_modifiers;

typedef struct print_data {
  char *line;
  char *filename;
  ssize_t line_len;
  size_t lines_counter;
  size_t matches_counter;
  int rc;
  int ovector[300];
  int end;
  int line_changed;
} print_data;

char *extend_pattern(char *old, char *add);
char *extend_pattern_from_file(char *old, char *filename);

int get_regexes(regexes *regs, match_modifiers *mods);
void free_regexes(regexes *regs);

int get_pattern(match_modifiers *mods, regexes *regs, input_data *input);
int find_matches_in_file(match_modifiers *mods, FILE *file, char *filename,
                         regexes *regs);

int re_find_match(match_modifiers *mods, print_data *data, regexes *regs,
                  int offset, ssize_t sub_len, size_t re_order);

void print_matches(match_modifiers *mods, print_data *data);
void print_prefix(match_modifiers *mods, print_data *data);
void print_match(match_modifiers *mods, print_data *data);
void print_score(match_modifiers *mods, print_data *data);

char *remove_first(input_data *input);

void add_filename(input_data *input, char *filename);

#endif  // _SRC_GREP_S21_GREP_H_
