#ifndef _SRC_GREP_S21_GREP_H_
#define _SRC_GREP_S21_GREP_H_
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pcre.h>

typedef struct match_modifiers {
    int argc;
    char **argv;
    int pcre_opts;
    int inversion;
    size_t matches_counter;
    size_t lines_counter;
    int only_matches_count;
    int print_line_number;
    int first_match;
    int hide_filenames;
    int hide_warnings;
    int all_matches;
} match_modifiers;

void extend_pattern(char *old, char *add);
void extend_pattern_from_file(char *old, char *filename);

char *get_pattern(match_modifiers *mods);
int find_matches_in_file(
    match_modifiers *mods,
    FILE *file,
    char *filename,
    pcre *re);

int find_match_in_line(
    match_modifiers *mods, pcre *re,
    char *str, size_t str_len);

void print_matches(char *subject, int *ovector, int rc);

#endif  // _SRC_GREP_S21_GREP_H_