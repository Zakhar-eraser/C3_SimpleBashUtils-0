#ifndef _SRC_GREP_S21_GREP_H_
#define _SRC_GREP_S21_GREP_H_
#include <pcre.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct match_modifiers {
  int argc;
  char **argv;
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
  char *subject;
  char *line;
  char *filename;
  ssize_t subject_len;
  ssize_t line_len;
  size_t lines_counter;
  size_t matches_counter;
  int rc;
  int ovector[300];
  int repeat;
  int lineChanged;
} print_data;

char *extend_pattern(char *old, char *add);
char *extend_pattern_from_file(char *old, char *filename);

pcre **get_regexes(char *pattern, match_modifiers *mods);

char *get_pattern(match_modifiers *mods, int *argind);
void find_matches_in_file(match_modifiers *mods, FILE *file, char *filename,
                          pcre *re);

int find_match_in_line(pcre *re, print_data *data);

size_t len(char *str);

void print_matches(match_modifiers *mods, print_data *data);
void print_score(match_modifiers *mods, print_data *data);

#endif  // _SRC_GREP_S21_GREP_H_
