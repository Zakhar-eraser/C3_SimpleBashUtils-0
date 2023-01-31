#include "s21_grep.h"

int main(int argc, char **argv) {
  int out = 0;
  regexes regs = {.pattern = NULL, .ptrn_cnt = 0, .res = NULL};
  match_modifiers mods = {0};
  input_data input = {.argc = argc, .argv = argv, .filenames = NULL};
  int ptrn_out = get_pattern(&mods, &regs, &input);
  switch (ptrn_out) {
    case 1:
      printf("Usage: s21_grep [options] template [file_name]");
      out = 2;
      break;
    case 2:
      printf("No such file: %s", optarg);
      out = 3;
      break;
    default:
      if (get_regexes(&regs, &mods)) {
        for (size_t i = 0; i < input.files_count; i++) {
          FILE *file;
          if ((file = fopen(input.filenames[i], "r"))) {
            out |= !find_matches_in_file(&mods, file, input.filenames[i], &regs);
            fclose(file);
          } else {
            out = 1;
            if (!(mods.hide_warnings)) printf("No such file: %s\n", input.filenames[i]);
          }
        }
      } else {
        printf("Pattern compilation error at %d: %s\n", regs.erroroffset, regs.error);
        out = 4;
      }
      break;
  }
  free_regexes(&regs);
  free(input.filenames);
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
    regs->res[regs->ptrn_cnt - 1] = pcre_compile(regs->pattern, mods->pcre_opts, &(regs->error), &(regs->erroroffset), NULL);
    if (regs->res[regs->ptrn_cnt - 1]) {
      reg = strtok(NULL, delim);
    } else {
      out = 0;
    }
  } while (reg && out);
  return out;
}

int find_matches_in_file(match_modifiers *mods, FILE *file, char *filename,
                          regexes *regs) {
  print_data data = {.filename = filename, .line = NULL,
                     .lines_counter = 0, .matches_counter = 0,
                     .line_len = 0};
  size_t line_cap = 0;
  int skip_file = 0, out = 0;
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
      out |= matched;
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
  return out;
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

int get_pattern(match_modifiers *mods, regexes *regs, input_data *input) {
  int out = 0;
  char *options = "e:ivclnhsf:o";
  while ((optind < input->argc) && !out) {
    int opt = getopt(input->argc, input->argv, options);
    if (opt == 'e') {
      regs->pattern = extend_pattern(regs->pattern, optarg);
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
      if (!(regs->pattern = extend_pattern_from_file(regs->pattern, optarg))) out = 2;
    } else if (opt == 'o') {
      mods->all_matches = 1;
    } else if (opt == -1) {
      add_filename(input, input->argv[optind]);
      optind++;
    } else {
      out = 1;
    }
  }
  if (!(regs->pattern)) {
    if (input->files_count > 1) {
      char *missed_pattern = remove_first(input);
      regs->pattern = extend_pattern(regs->pattern, missed_pattern);
    } else {
      out = 1;
    }
  }
  if (mods->all_matches && mods->inversion) mods->all_matches = 0;
  if (input->files_count == 1) mods->hide_filenames = 1;
  return out;
}

char *extend_pattern(char *old, char *add) {
  if (old) {
    char *tmp = (char *)realloc(old, sizeof(char) * (strlen(old) + strlen(add) + 2));
    if (tmp) {
      strcat(tmp, "|");
      strcat(tmp, add);
      old = tmp;
    }
  } else {
    old = (char *)calloc(strlen(add) + 1, sizeof(char));
    strcat(old, add);
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
    free(old);
    old = NULL;
  }
  return old;
}

void add_filename(input_data *input, char *filename) {
  char **tmp = (char **)realloc(input->filenames, sizeof(char *) * (input->files_count + 1));
  if (tmp) {
    tmp[input->files_count] = filename;
    input->files_count += 1;
    input->filenames = tmp;
  }
}

char *remove_first(input_data *input) {
  char *removing = NULL;
  char **filenames = input->filenames;
  size_t count = input->files_count;
  if (count) {
    removing = filenames[0];
    for (char **i = filenames; i < filenames + count - 1; i++) {
      *i = *(i + 1);
    }
    char **tmp = (char **)realloc(input->filenames, sizeof(char *) * (input->files_count - 1));
    count--;
    if (tmp) {
      input->filenames = tmp;
    } else if (!count) {
      input->filenames = NULL;
    } else {
      count++;
    }
    input->files_count = count;
  }
  return removing;
}

void free_regexes(regexes *regs) {
  for (pcre **re = regs->res; re < regs->res + regs->ptrn_cnt; re++) {
    pcre_free(*re);
  }
  free(regs->res);
  regs->res = NULL;
}
