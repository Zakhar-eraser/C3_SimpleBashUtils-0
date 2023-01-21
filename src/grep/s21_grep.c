#include "s21_grep.h"

int main(int argc, char **argv) {
  int out = 0, argind;
  regexes regs = {.pattern = NULL, .ptrn_cnt = 0, .res = NULL};
  match_modifiers mods = {.argc = argc, .argv = argv,
                          .all_matches = 0, .first_match = 0, .hide_filenames = 0,
                          .hide_warnings = 0, .inversion = 0, .only_matches_count = 0,
                          .pcre_opts = 0, .print_line_number = 0};
  regs.pattern = get_pattern(&mods, &argind);
  if (regs.pattern && (argind < argc)) {
    if (get_regexes(&regs, &mods)) {
      for (int i = argind; i < argc; i++) {
        FILE *file;
        if ((file = fopen(argv[i], "r"))) {
          find_matches_in_file(&mods, file, argv[i], &regs);
          fclose(file);
        } else {
          out = 1;
          if (!(mods.hide_warnings)) printf("No such file: %s\n", argv[i]);
        }
      }
    } else {
      out = 2;
    }
  } else {
    printf("Usage: s21_grep [options] template [file_name]");
    out = 2;
  }
  free_regexes(&regs);
  free(regs.pattern);
  regs.pattern = NULL;
  return out;
}

int get_regexes(regexes *regs, match_modifiers *mods) {
  int out = 1;
  char delim[2] = "|";
  if (!mods->all_matches) delim[0] = '\0';
  char *reg = strtok(regs->pattern, delim);
  regs->ptrn_cnt = 0;
  do {
    pcre **tmp = (pcre **)realloc(regs->res, sizeof(pcre *) * (regs->ptrn_cnt + 1));
    if (tmp) {
      regs->res = tmp;
      regs->ptrn_cnt++;
    }
    const char *error;
    int erroroffset;
    regs->res[regs->ptrn_cnt - 1] = pcre_compile(regs->pattern, mods->pcre_opts, &error, &erroroffset, NULL);
    if (regs->res[regs->ptrn_cnt - 1]) {
      reg = strtok(NULL, delim);
    } else {
      out = 0;
      printf("Pattern compilation error at %d: %s\n", erroroffset, error); 
    }
  } while (reg && out);
  return out;
}

void find_matches_in_file(match_modifiers *mods, FILE *file, char *filename,
                          regexes *regs) {
  print_data data = {.filename = filename, .line = NULL,
                     .lines_counter = 0, .matches_counter = 0,
                     .line_len = 0};
  size_t line_cap = 0;
  int skip_file = 0;
  while (((data.line_len = getline(&(data.line), &line_cap, file)) > 0) &&
         !skip_file) {
    data.repeat = 1;
    data.ovector[0] = -1;
    data.ovector[1] = 0;
    data.offset = 0;
    data.lines_counter++;
    data.lineChanged = 1;
    int skip_line = 0;
    size_t ptrn_order = 0;
    do {
      int matched = find_match_in_line(*(regs->res + ptrn_order), &data);
      skip_file = matched && mods->first_match;
      skip_line = matched && (!(mods->all_matches) || mods->only_matches_count);
      if ((matched && !(mods->inversion)) || (!matched && mods->inversion)) {
        data.matches_counter++;
        print_matches(mods, &data);
      }
      if (!data.repeat && (ptrn_order < regs->ptrn_cnt - 1)) {
        data.ovector[0] = -1;
        data.ovector[1] = 0;
        data.repeat = 1;
        ptrn_order++;
      }
    } while (data.repeat && !skip_file && !skip_line);
  }
  free(data.line);
  data.line = NULL;
  print_score(mods, &data);
}

int find_match_in_line(pcre *re, print_data *data) {
  int out = 0;
  int *ovector = data->ovector;
  int options = 0;
  if (ovector[0] == ovector[1]) {
    if (data->offset == data->line_len) {
      data->repeat = 0;
    } else {
      options = PCRE_NOTEMPTY_ATSTART | PCRE_ANCHORED;
    }
  }
  if (data->repeat) {
    data->rc = pcre_exec(re, NULL, data->line, data->line_len,
                         data->offset, options, ovector, 300);
    if (data->rc == PCRE_ERROR_NOMATCH) {
      if (options) {
        data->offset += 1;
      } else {
        data->repeat = 0;
      }
    }
    if (data->rc > 0) {
      out = 1;
      data->offset = ovector[1];
    }
  }
  return out;
}

void print_matches(match_modifiers *mods, print_data *data) {
  if (!(mods->only_matches_count) && !(mods->first_match)) {
    if (data->lineChanged) {
      if (!(mods->hide_filenames)) printf("%s:", data->filename);
      if (mods->print_line_number) printf("%lu:", data->lines_counter);
      data->lineChanged = 0;
    }
    if (mods->all_matches) {
      for (int i = 0; i < data->rc; i++) {
        char *sub_start = data->line + data->ovector[2 * i];
        int sub_len = data->ovector[2 * i + 1] - data->ovector[2 * i];
        if (sub_len) printf("%.*s\n", sub_len, sub_start);
      }
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
  if (mods->all_matches && mods->inversion) mods->all_matches = 0;
  return pattern;
}

char *extend_pattern(char *old, char *add) {
  size_t pref_len = len(old);
  char add_sign[2] = {0, 0};
  if (pref_len) {
    pref_len++;
    add_sign[0] = '|';
  }
  char *tmp = (char *)realloc(old, sizeof(char) * (pref_len + 1 + len(add)));
  tmp[0] = 0;
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
    }
    free(line);
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

void free_regexes(regexes *regs) {
  for (pcre **re = regs->res; re < regs->res + regs->ptrn_cnt; re++) {
    pcre_free(*re);
  }
  free(regs->res);
  regs->res = NULL;
}
