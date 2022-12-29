#include "s21_grep.h"

int main(int argc, char **argv) {
  int out = 0, erroroffset, argind;
  const char *error;
  pcre *re = NULL;
  match_modifiers mods = {.argc = argc, .argv = argv};
  char *pattern = get_pattern(&mods, &argind);
  if (pattern && (argind < argc)) {
    if ((re = pcre_compile(pattern, mods.pcre_opts, &error, &erroroffset,
                           NULL))) {
      if (!(mods.all_matches && mods.inversion)) {
        for (int i = argind; i < argc; i++) {
          FILE *file;
          if ((file = fopen(argv[i], "r"))) {
            find_matches_in_file(&mods, file, argv[i], re);
            fclose(file);
          } else {
            out = 1;
            if (!(mods.hide_warnings)) printf("No such file: %s\n", argv[i]);
          }
        }
      }
    } else {
      out = 2;
      printf("Pattern compilation error at %d: %s\n", erroroffset, error);
    }
  } else {
    printf("Usage: s21_grep [options] template [file_name]");
    out = 2;
  }
  if (re) pcre_free(re);
  free(pattern);
  return out;
}

void find_matches_in_file(match_modifiers *mods, FILE *file, char *filename,
                          pcre *re) {
  print_data data = {.filename = filename, .line = NULL};
  size_t line_cap = 0;
  int skip_file = 0;
  while ((data.line_len = getline(&(data.line), &line_cap, file)) > 0 &&
         !skip_file) {
    data.repeat = mods->all_matches;
    data.ovector[0] = 0;
    data.ovector[1] = 1;
    data.subject = data.line;
    data.subject_len = data.line_len;
    data.lines_counter++;
    int skip_line;
    do {
      int matched = find_match_in_line(re, &data);
      skip_file = matched && mods->first_match;
      skip_line = matched && mods->only_matches_count;
      if ((matched && !(mods->inversion)) || (!matched && mods->inversion)) {
        data.matches_counter++;
        print_matches(mods, &data);
      }
    } while (data.repeat && !skip_file && !skip_line);
    free(data.line);
    data.line = NULL;
  }
  print_score(mods, &data);
}

int find_match_in_line(pcre *re, print_data *data) {
  int out = 0, opts = 0;
  int *ovector = data->ovector;
  if (ovector[0] == ovector[1]) {
    if (ovector[0] == (int)(data->subject_len)) {
      data->repeat = 0;
    } else {
      opts = PCRE_NOTEMPTY_ATSTART | PCRE_ANCHORED;
    }
  }
  data->rc = pcre_exec(re, NULL, data->subject, data->subject_len, 0, opts,
                       ovector, 300);
  if ((data->rc == PCRE_ERROR_NOMATCH) && (opts == 0)) data->repeat = 0;
  if (data->rc > 0) out = 1;
  return out;
}

void print_matches(match_modifiers *mods, print_data *data) {
  if (!(mods->only_matches_count) && !(mods->first_match)) {
    if (!(mods->hide_filenames)) printf("%s:", data->filename);
    if (mods->print_line_number) printf("%lu:", data->lines_counter);
    if (mods->all_matches) {
      for (int i = 0; i < data->rc; i++) {
        char *sub_start = data->subject + data->ovector[2 * i];
        int sub_len = data->ovector[2 * i + 1] - data->ovector[2 * i];
        printf("%.*s\n", sub_len, sub_start);
      }
      int skip = data->ovector[data->rc * 3 - 2];
      data->subject += skip;
      data->subject_len -= skip;
    } else {
      printf("%s", data->line);
      if (data->line[data->line_len - 1] != '\n') printf("\n");
    }
  }
}

void print_score(match_modifiers *mods, print_data *data) {
  if (mods->first_match && data->matches_counter) {
    printf("%s\n", data->filename);
  } else {
    if (mods->only_matches_count) {
      if (!(mods->hide_filenames)) printf("%s:", data->filename);
      printf("%lu\n", data->matches_counter);
    }
  }
}

char *get_pattern(match_modifiers *mods, int *argind) {
  int opt, cont = 1;
  char *options = "e:ivclnhsf:o", *pattern = NULL;
  while (((opt = getopt(mods->argc, mods->argv, options)) != -1) && cont) {
    if (opt == 'e') {
      pattern = extend_pattern(pattern, optarg);
    } else if (opt == 'i') {
      mods->pcre_opts = PCRE_CASELESS;
    } else if (opt == 'v') {
      mods->inversion = 1;
    } else if (opt == 'c') {
      mods->only_matches_count = 1;
    } else if (opt == 'l') {
      mods->first_match = 1;
    } else if (opt == 'n') {
      mods->print_line_number = 1;
    } else if (opt == 'h') {
      mods->hide_filenames = 1;
    } else if (opt == 's') {
      mods->hide_warnings = 1;
    } else if (opt == 'f') {
      if (!(pattern = extend_pattern_from_file(pattern, optarg))) cont = 0;
    } else if (opt == 'o') {
      mods->all_matches = 1;
    } else {
      cont = 0;
      printf("Wrong options\n");
      free(pattern);
      pattern = NULL;
    }
  }
  *argind = optind;
  if (!len(pattern) && (optind < mods->argc - 1)) {
    pattern = extend_pattern(pattern, mods->argv[optind]);
    (*argind)++;
  }
  if (mods->argc - *argind == 1) mods->hide_filenames = 1;
  return pattern;
}

char *extend_pattern(char *old, char *add) {
  size_t pref_len = len(old);
  char add_sign[2] = {0, 0};
  if (pref_len) {
    pref_len++;
    add_sign[0] = '|';
  }
  char *tmp = (char *)realloc(old, sizeof(char) * (pref_len + len(add)));
  if (tmp) {
    strcat(tmp, add_sign);
    strcat(tmp, add);
    old = tmp;
  }
  return old;
}

char *extend_pattern_from_file(char *old, char *filename) {
  FILE *file = fopen(filename, "r");
  if (file) {
    size_t line_cap = 0;
    char *line = NULL;
    ssize_t line_len;
    while ((line_len = getline(&line, &line_cap, file)) > 0) {
      if (line[line_len - 1] == '\n') line[line_len - 1] = '\0';
      old = extend_pattern(old, line);
      free(line);
      line = NULL;
    }
    fclose(file);
  } else {
    printf("No such file: %s", filename);
    free(old);
    old = NULL;
  }
  return old;
}

size_t len(char *str) {
  size_t l = 0;
  if (str) l = strlen(str);
  return l;
}
